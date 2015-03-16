all: ../tutorial.pdf

../tutorial.pdf: tutorial.pdf
	cp $< ../

tutorial.pdf: tutorial.tex
	pdflatex $<

clean:
	rm -f *.aux *.log *.out *.pdf
