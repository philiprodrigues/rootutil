LIBRARIES := librootutil-tree.so librootutil-plot.so
CXXFLAGS := `root-config --cflags` -fPIC -I.
LDFLAGS := `root-config --ldflags --libs --glibs` -lTreePlayer

all: $(LIBRARIES)

%.o: %.C
	$(CXX) $(INCLUDE) $(CXXFLAGS) -MMD -MP -o $@ -c $<

%.o: %.cxx
	$(CXX) $(INCLUDE) $(CXXFLAGS) -MMD -MP -o $@ -c $<

librootutil-tree.so: tree/TreeWrapper.o tree/ChainWrapper.o tree/TreeDict.o
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

librootutil-plot.so: plot/drawWithCutAxis.o  plot/GridCanvas.o plot/PlotDict.o
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

tree/TreeDict.cxx: tree/TreeWrapper.h tree/ChainWrapper.h tree/LinkDef.h
	rootcint -f $@ -c $(CXXFLAGS) -p $^

plot/PlotDict.cxx: plot/GridCanvas.h plot/LinkDef.h
	rootcint -f $@ -c $(CXXFLAGS)  -p $^ 

clean:
	rm -f tree/*.o *.so tree/TreeDict* plot/*.o plot/PlotDict*

.PHONY: clean
