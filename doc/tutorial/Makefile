all: ../tutorial.pdf

../tutorial.pdf: tutorial.pdf
	cp $< ../

tutorial.pdf: tutorial.tex
	pdflatex $<
	pdflatex $<
	pdflatex $<

clean:
	rm -f *.blg *.bbl *.aux *.log *.out *.pdf
