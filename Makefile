LIBRARIES := librootutil-tree.so librootutil-plot.so
CFLAGS := `root-config --cflags` -fPIC
LDFLAGS := `root-config --ldflags --libs --glibs` -lTreePlayer

all: $(LIBRARIES)

%.o: %.C
	$(CC) $(INCLUDE) $(CFLAGS) -MMD -MP -o $@ -c $<

%.o: %.cxx
	$(CXX) $(INCLUDE) $(CFLAGS) -MMD -MP -o $@ -c $<

librootutil-tree.so: tree/TreeWrapper.o tree/ChainWrapper.o
	$(CXX) -shared -o $@ $< $(LDFLAGS)

librootutil-plot.so: plot/drawWithCutAxis.o  plot/GridCanvas.o
	$(CXX) -shared -o $@ $< $(LDFLAGS)