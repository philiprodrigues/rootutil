void add_inc_path(TString path)
{
  TString oldpath=gSystem->GetIncludePath();
  oldpath+=" -I";
  oldpath+=path;
  gSystem->SetIncludePath(oldpath);
}

void loadTreeWrapper()
{
  add_inc_path("/home/rodrigues/dune/daq/primsim/util/tree");
  gSystem->CompileMacro("TreeWrapper.cxx", "k");
  gSystem->CompileMacro("ChainWrapper.cxx", "k");
}
