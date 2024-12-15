# include <string>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
using namespace std;
void print_version()
{
  const string VERSION="0.4";
  const string PATCHLEVEL="4";
  const string DATE="2007-01-18";
  const string REVISION="35";
cout<<"tab, Version "<<VERSION<<"."<<PATCHLEVEL<<"  (Last SVN Revision "<<REVISION<<"), "<<DATE<<"\n"<<"Written by Kai Berggold\n";
 exit(-1);
};

