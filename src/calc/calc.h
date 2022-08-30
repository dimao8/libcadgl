#ifndef CALC_H
#define CALC_H

#include <string>
#include <vector>

namespace cadgl
{
  /**
   * Mathematic expression token type
   */
  enum calc_token_type_t
  {
    tok_eof,
    tok_operator,
    tok_number,
    tok_function
  };

  /**
   * Mathematic expression token
   */
  struct calc_token
  {
    calc_token_type_t token_type;
    std::string token_value;
    float token_float;

    calc_token(calc_token_type_t type, const std::string& val)
    : token_type(type),
      token_value(val)
    {
      if (type == tok_number)
        token_float = std::stof(val);
    }

    calc_token(const calc_token& tok)
    : token_type(tok.token_type),
      token_value(tok.token_value) 
    {
      if (tok.token_type == tok_number)
        token_float = std::stof(tok.token_value);
    }
  };

  /**
   * Token vector
   */
  typedef std::vector<calc_token> token_list;

  /**
   * calc is an miniature calculator to resolve small mathematical expressions.
   * 
   * calc can use four standard operators +, -, *, / and power operator as ^.
   * Also calc can resolve internal functions like
   * - sqrt()
   * - sin()
   * - cos()
   * - tan()
   * - ctg()
   * - ln()
   * 
   * The numbers must look like one of the examples with . or , as fractional separator
   * 
   * - 125
   * - 58.12
   * - 0.256
   * - 0.256e3
   * - 0.1255E-8
   * - 25E23
   * 
   */
  class calc
  {

  private:

    token_list m_tokens;

    void reset();
    bool lexical(const std::string& expr);
    bool syntax(float* result);
    float parse_expression(token_list& list, token_list::iterator& input);
    float parse_term1(token_list& list, token_list::iterator& input);
    float parse_term2(token_list& list, token_list::iterator& input);
    float parse_term3(token_list& list, token_list::iterator& input);
    float parse_term4(token_list& list, token_list::iterator& input);
    float parse_term5(token_list& list, token_list::iterator& input);

  public:
    calc();
    ~calc();

    bool resolve(float* result, const std::string& expr);

  };
} // namespace cadgl


#endif // CALC_H
