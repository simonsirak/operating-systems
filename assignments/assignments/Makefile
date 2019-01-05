PDFLATEX = pdflatex

DIRS = 	boot buddy communication contexts files fork green lkm locks magic mylloc maplloc signals stack storage swapping tlb 

WWW =  ~/afs/public_html/ose/assignments


all:
	echo $(DIRS)
	for d in $(DIRS); do  echo ./$$d; cd $$d && make && cd ..; \
        done


install: 
	for d in $(DIRS); \
             do rsync -u ./$$d/$$d.pdf $(WWW)/$$d.pdf;\
        done


clean : 
	for d in $(DIRS); do cd ./$$d && make clean && cd ..; \
        done

realclean : 
	for d in $(DIRS); do cd ./$$d && make realclean && cd ..; \
        done


