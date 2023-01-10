#include "calc.h"

#include <iostream>
#include <cmath>

namespace cadgl
{

  /* ******************************* calc::calc ******************************* */

  calc::calc()
  {
    // TODO : Constructor
  }

  /* ******************************* calc::~calc ****************************** */

  calc::~calc()
  {
    // TODO : Destructor
  }

  /* ******************************* calc::reset ****************************** */

  void calc::reset()
  {
    m_tokens.clear();
  }

  /* ****************************** calc::resolve ***************************** */

  bool calc::resolve(float* result, const std::string& expr)
  {
    reset();

    if (!lexical(expr))
      return false;

    for (auto it : m_tokens)
      {
        switch (it.token_type)
          {

          case tok_eof:
            std::cout << "eof: ";
            break;

          case tok_function:
            std::cout << "function: ";
            break;

          case tok_number:
            std::cout << "number: ";
            break;

          case tok_operator:
            std::cout << "operator: ";
            break;

          }
        
        if (it.token_type == tok_number)
          std::cout << it.token_float << std::endl;
        else  
          std::cout << it.token_value << std::endl;
      }

    if (!syntax(result))
      return false;

    return true;
  }

  /* ****************************** calc::lexical ***************************** */

  bool calc::lexical(const std::string& expr)
  {
    int idx = 0;
    std::string str;

    std::cout << "lexical" << std::endl;

    if (expr.empty())
      {
        std::cout << "Syntax error at 0: (empty)" << std::endl;
        return false;
      }

    while (true)
      {
        if (expr[idx] == '+'
          || expr[idx] == '-'
          || expr[idx] == '/'
          || expr[idx] == '*'
          || expr[idx] == '('
          || expr[idx] == ')'
          || expr[idx] == '^')
          {
            str.clear();
            str.push_back(expr[idx]);
            m_tokens.emplace_back(tok_operator, str);
            idx++;
          }
        else if (std::isdigit(expr[idx]))
          {
            // There is no - operator so skip first sign
            // Get integer part
            str.clear();
            while (std::isdigit(expr[idx]))
              {
                str.push_back(expr[idx]);
                idx++;
              }

            // Check for separator. Replace it with .
            if (expr[idx] == '.' || expr[idx] == ',')
              {
                str.push_back('.');
                idx++;

                // Check for fraction part
                if (std::isdigit(expr[idx]))
                  {
                    while (std::isdigit(expr[idx]))
                      {
                        str.push_back(expr[idx]);
                        idx++;
                      }
                  }
                else
                  {
                    std::cout << "Syntax error at " << idx << ": (fraction expected)" << std::endl;
                    return false;
                  }
              }

            // Check for e or E
            if (expr[idx] == 'e' || expr[idx] == 'E')
              {
                str.push_back('e');
                idx++;

                // Check for sign
                if (expr[idx] == '-')
                  {
                    str.push_back('-');
                    idx++;
                  }

                // Check for rest of exponent
                if (std::isdigit(expr[idx]))
                  {
                    while (std::isdigit(expr[idx]))
                      {
                        str.push_back(expr[idx]);
                        idx++;
                      }
                  }
                else
                  {
                    std::cout << "Syntax error at " << idx << ": (exponent expected)" << std::endl;
                    return false;
                  }
              }

            m_tokens.emplace_back(tok_number, str);
          }
        else if (std::isalpha(expr[idx]))
          {
            str.clear();
            while (std::isalpha(expr[idx]))
              {
                str.push_back(expr[idx]);
                idx++;
              }
            m_tokens.emplace_back(tok_function, str);
          }
        else if (expr[idx] == 0)
          break;
        else
          {
            std::cout << "Syntax error at " << idx << ": (unknown token)" << std::endl;
            return false;
          }
      }

    return true;
  }

  /* ****************************** calc::syntax ****************************** */

  bool calc::syntax(float* result)
  {
    token_list::iterator it = m_tokens.begin();

    *result = parse_expression(m_tokens, it);
    if (isinff(*result))
      return false;
    else
      return true;
  }

  /* ************************* calc::parse_expression ************************* */

  float calc::parse_expression(token_list& list, token_list::iterator& input)
  {
    std::cout << "expression starts from (" << input->token_value << ")" << std::endl;
    float f = parse_term1(list, input);
    std::cout << "back to expression (" << f << ")" << std::endl;

    if (input == list.end())                      // This is the last token
      return f;

    if (input->token_type == tok_operator)
      {
        switch (input->token_value[0])
          {

          case '+':
            std::cout << "this is ``+\'\'" << std::endl;
            input++;
            f += parse_expression(list, input);
            std::cout << "back to expression (" << f << ")" << std::endl;
            return f;

          case '-':
            std::cout << "this is ``-\'\'" << std::endl;
            input++;
            f -= parse_expression(list, input);
            std::cout << "back to expression (" << f << ")" << std::endl;
            return f;

          case ')':
            std::cout << "return from ``)\'\'" << std::endl;
            return f;

          }
      }

    std::cout << "*** Ouch in expression (" << input->token_value << ")" << std::endl;
    return INFINITY;
  }

  float calc::parse_term1(token_list& list, token_list::iterator& input)
  {
    std::cout << "term1 starts from (" << input->token_value << ")" << std::endl;
    float f = parse_term2(list, input);
    std::cout << "back to term1 (" << f << ")" << std::endl;

    if (input == list.end())                      // This is the last token
      return f;

    if (input->token_type == tok_operator)
      {
        switch (input->token_value[0])
          {

          case '*':
            std::cout << "this is ``*\'\'" << std::endl;
            input++;
            f *= parse_term1(list, input);
            std::cout << "back to term1 (" << f << ")" << std::endl;
            return f;

          case '/':
            std::cout << "this is ``/\'\'" << std::endl;
            input++;
            f /= parse_term1(list, input);
            std::cout << "back to term1 (" << f << ")" << std::endl;
            return f;

          }
      }

    return f;
  }

  float calc::parse_term2(token_list& list, token_list::iterator& input)
  {
    std::cout << "term2 starts from (" << input->token_value << ")" << std::endl;
    float f = parse_term3(list, input);
    std::cout << "back to term2 (" << f << ")" << std::endl;

    if (input == list.end())                      // This is the last token
      return f;

    if (input->token_type == tok_operator)
      {
        switch (input->token_value[0])
          {

          case '^':
            std::cout << "this is ``^\'\'" << std::endl;
            input++;
            f = powf(f, parse_term2(list, input));
            std::cout << "back to term2 (" << f << ")" << std::endl;
            return f;

          }
      }

    return f;
  }

  float calc::parse_term3(token_list& list, token_list::iterator& input)
  {
    float f;

    std::cout << "term3 starts from (" << input->token_value << ")" << std::endl;

    if (input == list.end())                      // This is the last token
      return INFINITY;

    if (input->token_type == tok_operator)
      {
        switch (input->token_value[0])
          {

          case '-':
            std::cout << "this is unary ``-\'\'" << std::endl;
            input++;
            f = -parse_term3(list, input);
            std::cout << "back to term3 (" << f << ")" << std::endl;
            return f;

          }
      }

    f = parse_term4(list, input);
    std::cout << "back to term3 (" << f << ")" << std::endl;
    return f;
  }

  float calc::parse_term4(token_list& list, token_list::iterator& input)
  {
    float f;

    std::cout << "term4 starts from (" << input->token_value << ")" << std::endl;

    if (input == list.end())                      // This is the last token
      return INFINITY;

    if (input->token_type == tok_function)
      {
        if (input->token_value == "sin")
          {
            std::cout << "this is ``sin\'\'" << std::endl;
            input++;
            f = sinf(parse_term4(list, input));
            std::cout << "back to term4 (" << f << ")" << std::endl;
            return f;
          }
        else if (input->token_value == "cos")
          {
            std::cout << "this is ``cos\'\'" << std::endl;
            input++;
            f = cosf(parse_term4(list, input));
            std::cout << "back to term4 (" << f << ")" << std::endl;
            return f;
          }
        else if (input->token_value == "tan")
          {
            std::cout << "this is ``tan\'\'" << std::endl;
            input++;
            f = tanf(parse_term4(list, input));
            std::cout << "back to term4 (" << f << ")" << std::endl;
            return f;
          }
        else if (input->token_value == "ctg")
          {
            std::cout << "this is ``ctg\'\'" << std::endl;
            input++;
            f = 1.0f/tanf(parse_term4(list, input));
            std::cout << "back to term4 (" << f << ")" << std::endl;
            return f;
          }
        else if (input->token_value == "sqrt")
          {
            std::cout << "this is ``sqrt\'\'" << std::endl;
            input++;
            f = sqrtf(parse_term4(list, input));
            std::cout << "back to term4 (" << f << ")" << std::endl;
            return f;
          }
        else if (input->token_value == "ln")
          {
            std::cout << "this is ``ln\'\'" << std::endl;
            input++;
            f = logf(parse_term4(list, input));
            std::cout << "back to term4 (" << f << ")" << std::endl;
            return f;
          }
        else
          {
            return INFINITY;
          }
      }

    f = parse_term5(list, input);
    std::cout << "back to term4 (" << f << ")" << std::endl;
    return f;
  }

  float calc::parse_term5(token_list& list, token_list::iterator& input)
  {
    float f;

    std::cout << "term5 starts from (" << input->token_value << ")" << std::endl;

    if (input == list.end())                      // This is the last token
      return INFINITY;

    if (input->token_type == tok_operator && input->token_value[0] == '(')
      {
        input++;
        f = parse_expression(list, input);
        std::cout << "back to term5 (" << f << ")" << std::endl;
        if (input != list.end() && input->token_value[0] == ')')
          input++;
        else
          return INFINITY;
        std::cout << "this is expression in ``()\'\'" << std::endl;
        return f;
      }

    if (input->token_type == tok_number)
      {
        f = input->token_float;
        input++;
        std::cout << "this is a number (" << f << ")" << std::endl;
        return f;
      }

    return INFINITY;
  }

} // namespace cadgl
