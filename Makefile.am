ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libcadgl.la
libcadgl_la_SOURCES = src/cadgl.cpp \
	src/calc/calc.cpp \
	src/calc/calc.h \
	src/graphic/canvas.cpp \
	src/graphic/canvas.h
libcadgl_la_CXXFLAGS = -I$(srcdir)/include

include_HEADERS = include/cadgl.h

CLEANFILES =
SUFFIXES =

# Documents
if BUILD_DOC
dist_doc_DATA = doc/description_ru.tex

SUFFIXES += .tex .aux .pdf
DOCDIR = doc
DOCNAME = $(DOCDIR)/description_ru
DOCPDF = $(DOCNAME).pdf
DOCSRC = $(DOCNAME).tex
DOCAUX = $(DOCNAME).aux
doc_DATA = $(DOCPDF)
PDFLATEXOPTS = --output-directory=$(DOCDIR)
CLEANFILES += $(DOCPDF) $(DOCNAME).log $(DOCNAME).aux $(DOCNAME).toc \
	$(DOCNAME).out $(DOCNAME).idx $(DOCNAME).ilg $(DOCNAME).ind $(DOCNAME).btmp

%.pdf : %.aux
	@echo === Making PDF: $@ from $^ ===
	$(MKDIR_P) $(DOCDIR)
	$(PDFLATEX) $(PDFLATEXOPTS) $(srcdir)/$(DOCSRC)
	@while ( grep "Rerun to " \
		$(DOCNAME).log ); do \
			echo '** Re-running LaTeX **';      \
			$(PDFLATEX) $(PDFLATEXOPTS) $(srcdir)/$(DOCSRC); \
	done
	if [ -f $(DOCNAME).idx]; then \
		echo === Making index ===; \
		makeindex $(DOCNAME); \
	fi
	$(PDFLATEX) $(PDFLATEXOPTS) $(srcdir)/$(DOCSRC)

%.aux : %.tex
	@echo === Making $@ file from $^ ===
	$(MKDIR_P) $(DOCDIR)
	$(PDFLATEX) $(PDFLATEXOPTS) $(srcdir)/$(DOCSRC)
endif

# Tests

TESTS = tests/test1 \
	tests/test2 \
	tests/test3 \
	tests/test4 \
	tests/test5 \
	tests/test6

XFAIL_TESTS = tests/test2 \
	tests/test4 \
	tests/test5

check_PROGRAMS = tests/testcalc \
	tests/testcanvas

tests_testcalc_SOURCES = tests/testcalc.cpp
tests_testcalc_CXXFLAGS = -I$(srcdir)/include
tests_testcalc_LDADD = -lcadgl

tests_testcanvas_SOURCES = tests/testcanvas.cpp
tests_testcanvas_CXXFLAGS = -I$(srcdir)/include
tests_testcanvas_LDADD = -lcadgl
