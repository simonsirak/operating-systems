PDFLATEX = pdflatex
LATEXMK = latexmk

all : magic.pdf


%.pdf : %.tex 
	$(LATEXMK) -pdf -pdflatex=$(PDFLATEX) -use-make $<


.PHONY : clean realclean

clean:
	$(LATEXMK) -c

realclean: clean
	rm -f *.pdf 




