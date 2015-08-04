CXX=g++

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

ROOTIL      = $(ROOTCFLAGS) $(ROOTLIBS)

LIBS            = libObjects.so

TopAnalysis:
	$(CXX) $(ROOTIL) -L ${TOOLSDIR}/lib  -l Objects -l MassAMWT -l LHAPDF -l JetMETObjects -I ${TOOLSDIR} -I ${LHAPDFINC} -O2 -Wall -o TopAnalysis TopAnalysis.C

TopPartnersv4:
	$(CXX) $(ROOTIL) -L ${TOOLSDIR}/lib  -l Objects -l MassAMWT -l LHAPDF -l JetMETObjects -I ${TOOLSDIR} -I ${LHAPDFINC} -O2 -Wall -o TopPartnersv4 TopPartnersv4.C

Yield:
	$(CXX) $(ROOTIL) -L ${TOOLSDIR}/lib  -l Objects -I ${TOOLSDIR} -o Yield Yield.C

YieldSA:
	$(CXX) $(ROOTIL) -l RooFit -l RooFitCore -O2 -Wall -o YieldSA YieldSA.C

YieldSANewest:
	$(CXX) $(ROOTIL) -O2 -Wall -o YieldSANewest YieldSANewest.C

MassFit_nine:
	$(CXX) $(ROOTIL) -l RooFit -l RooFitCore -O2 -Wall -o MassFit_nine MassFit_nine.C


DrawOneHistoWithSyst:
	$(CXX) $(ROOTIL) -O2 -Wall -o DrawOneHistoWithSyst DrawOneHistoWithSyst.C

MassFit_print:
	$(CXX) $(ROOTIL) -l RooFit -l RooFitCore -O2 -Wall -o MassFit_print MassFit_print.C

Systematics:
	$(CXX) $(ROOTIL) -O2 -Wall -o Systematics Systematics.C

PromptLnTProb:
	$(CXX) $(ROOTIL) -O2 -Wall -o PromptLnTProb PromptLnTProb.C

LooseTopPartnersv4:
	$(CXX) $(ROOTIL) -O2 -Wall -o LooseTopPartnersv4 LooseTopPartnersv4.C

GetFakeRate:
	$(CXX) $(ROOTIL) -O2 -Wall -o GetFakeRate GetFakeRate.C

IsoCheck:
	$(CXX) $(ROOTIL) -O2 -Wall -o IsoCheck IsoCheck.C

ErrorPropagation:
	$(CXX) $(ROOTIL) -O2 -Wall -o ErrorPropagation ErrorPropagation.C

clean: 
	rm *.o *.so *.d

YieldSA_pdf:
	$(CXX) $(ROOTIL) -l RooFit -l RooFitCore -O2 -Wall -o YieldSA_pdf YieldSA_pdf.C

all: TopPartnersv4 DataDrivenOS LooseTopPartnersv4

.PHONY: TopAnalysis
.PHONY: TopPartnersv4
.PHONY: Yield
.PHONY: YieldSA
.PHONY: YieldSANewest
.PHONY: MassFit_func
.PHONY: DrawOneHistoFR
.PHONY: DrawOneHistoDDB
.PHONY: DrawOneHistoWithSyst
.PHONY: MassFit_nine
.PHONY: Systematics
.PHONY: PromptLnTProb
.PHONY: LooseTopPartnersv4
.PHONY: GetFakeRate
.PHONY: IsoCheck
.PHONY: ErrorPropagation
