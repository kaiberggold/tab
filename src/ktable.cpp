# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <ctime>
# include <cmath>
# include <string>
# include <vector>
# include <algorithm>

using namespace std;

# include "ktable.h"
# include "linear.h"
ktable::ktable()
{
  m= new unsigned int;
  n=new unsigned int;
  *n=0;
  *m=0;
  header="";
  options.sort=0;
  options.average=0;
  options.value=0;
  options.reverse=false;
  options.line_windows=false;
  options.header_write=false;
  options.header_orig=false;
  options.precision=8;
  options.outname="";
  options.append=false;
  options.appendname="";
  options.factoropx1='*';
  options.factoropx2='*';
  options.factoropy1='*';
  options.factoropy2='*';
  
  
}


ktable::ktable(const ktable &a)
{
  unsigned int i;
  m= new unsigned int;
  n=new unsigned int;
  *n=0;
  *m=0;  
    for (i=0;i<*(a.n);i++)
  {
    add_row(a.table[i]);
  }
  
  header=a.header;
  options=a.options;
  
}

ktable::~ktable()
{
  if (!table.empty()) table.clear();
  delete m;
  delete n;
  
  
}
void ktable::set_options(char opt, int value)
{
  if (opt=='a') options.average=value;
  if (opt=='s') options.sort=value;
  if (opt=='r') options.reverse=value;
  if (opt=='l') options.line_windows=value;
  if (opt=='H') options.header_write=value;
  if (opt=='O') options.header_orig=value;
  if (opt=='p') options.precision=value;
  if (opt=='P') options.append=value;
  if (opt=='1') options.factoropx1=value;
  if (opt=='2') options.factoropy1=value;
  if (opt=='3') options.factoropx2=value;
  if (opt=='4') options.factoropy2=value;
  

}

int ktable::get_options(char opt)
{
  if (opt=='a') return options.average;
  if (opt=='s') return options.sort;
  if (opt=='r') return options.reverse;
  if (opt=='l') return options.line_windows;
  if (opt=='H') return options.header_write;
  if (opt=='O') return options.header_orig;
  if (opt=='p') return options.precision;
  if (opt=='P') return options.append;
  if (opt=='1') return options.factoropx1;
  if (opt=='2') return options.factoropy1;
  if (opt=='3') return options.factoropx2;
  if (opt=='4') return options.factoropy2;
  
  cerr<<"Intern error: get_options - unknown option";
  return 0;
}

string ktable::get_options_string(char opt)
{
  if (opt=='o') return options.outname;
  if (opt=='P') return options.appendname;
  cerr<<"Intern error: get_options_string - unknown option";
  return 0;
}



void ktable::set_options(double value)
{
  options.value=value;
}

void ktable::set_options_string(char opt, string value)
{
  if (opt=='o') options.outname=value;
  if (opt=='P') options.appendname=value;
  
}

double ktable::get_options()
{
  return options.value;
}






string  mystring::get_first_line(string in)
{
 if (in.find_first_of ("\r",0)!=string::npos)
    return in.substr(0,in.find_first_of("\r",0));
    
 if (in.find_first_of ('\n',0)!=string::npos)
    return in.substr(0,in.find_first_of("\n",0));
  return in;
}

string  mystring::remove_first_line(string in)
{
    
 if (in.find_first_of ('\n',0)!=string::npos)
    return in.substr(in.find_first_of("\n",0)+1,npos);
  return in;
}

 string  mystring::remove_first_character(string in)
{
    
 if (in=="") return in;
    return in.substr(1,npos);
  return in;
}


string mystring::remove_blanks (string in)
{
string changed_string;
changed_string=in;
if (changed_string.size()==0) {return "";}
do 
  {
  if (changed_string[0] ==' ')
    {
      changed_string=changed_string.substr(1);
      if (changed_string.size()==0) {return "";}
    }
  }
while (changed_string[0] ==' ');
return changed_string;

}

string mystring::remove_first_raute (string in)
{
//cerr<<"lala"<<in<<"\n";
  //cerr<< in.substr(1,npos)<<in.find_first_of ('#',0);
  if (in.find_first_of ('#',0)==0)
   {
    // cerr<<"k";
   
     return in.substr(1,npos);
   }
 return in;
}




int mystring::count_blanks (string in)
{
unsigned int i=0;
string changed_string;
changed_string=in;
if (changed_string.size()==0) {return 0;}
do 
  {
  if (changed_string[0] ==' ')
    {
      i++;
      changed_string=changed_string.substr(1);
      if (changed_string.size()==0) {return i;}
    }
  }
while (changed_string[0] ==' ');
return i;

}


string ktable::table_file_read(char *input_filename)

//******************************************************************************

{
  ifstream input;
  string returnstring;
  table.clear();
  vector <double> rvec;
  bool error, header_read=true;
  string line;
  int last,wcount;
  *n=0;
  *m=0;
  header="";
  input.open ( input_filename );
  if ( !input )
  {
 
    returnstring=input_filename;
    
    return "Couldn't open file: "+returnstring+"\n";
  }

  for (;;)
  { 
    getline ( input,line );
    if ( input.eof ( ) )
      {
        break;
      }
    line=remove_blanks(line);
    if ( s_len_trim ( line ) == 0 )
      {
        continue;
      }
    if ( line[0] == '#' )
      {
           
      if (header_read)
         {
           header=header+get_first_line(line)+line_end();
           
  //         cout <<"header=\n" << *header ;
         }
        continue;
     }
    if ( line[0] == '@' )
      {
        continue;
      }




      wcount=word_count(line);
      //cerr <<"wordcount = " <<wcount <<" \n";

      rvec.resize(wcount);
      error = s_to_dvec (line, wcount, &last,rvec);
      //cerr <<"last = " <<last <<" \n error = "<<error <<" \n";
      //rvec.clear();
      if (!error)
        {
         header_read=false;
        // cout <<"table: "<< *n <<"\n";
	 add_row(rvec);//exit(-1);
         //cerr<<"line:"<<line<<"\n";
         //for(int ii=0;ii<rvec.size();ii++) cerr <<rvec[ii]<<"\t";
         //cerr <<"\n";
	         //++*n;
 
	 //if (*m<last) 
        //{ 
          //  *m=last;
          //}
       }
  }
  string temp;
  if(*n==0) return "File "+returnstring+ " contains no data\n";
  else return "";
}




bool ktable::s_to_dvec (  string s, int n, int *last, vector<double> &rvec )

{
  bool error, first=false;
  int i, j=0;

  int lchar;
  *last=0;
  for ( i = 0; i < n; i++ )
  {
   //cout <<"i="<<i<<"\n";
    rvec[i] = s_to_d ( remove_blanks(s.substr(j)+" "), &lchar, &error );
   //cerr<<"rbl: "<<remove_blanks(s.substr(j))<<"\n";
   //<<"lchar "<<lchar<<"\n";
   //cerr <<"revec["<<i<<"]: "<<s_to_d ( remove_blanks(s.substr(j)+" "), &lchar, &error );
 
  
   if (i==0 && !error)
    {
    // cout <<"hallo";
      first=true;
      *last=1;
    }
    if (i!=0 && !error)
    {
     //cout <<"hwrefllo";
      *last=i+1;
    }
    j = j + lchar+count_blanks(s.substr(j));

  }

  return (!first);
}
//******************************************************************************



double ktable::s_to_d ( string s, int *lchar, bool *error )

//******************************************************************************
//
//  Purpose:
//
//    S_TO_D reads a real number from a string.
//
//  Discussion:
//
//    This routine will read as many characters as possible until it reaches
//    the end of the string, or encounters a character which cannot be
//    part of the real number.
//
//    Legal input is:
//
//       1 blanks,
//       2 '+' or '-' sign,
//       2.5 spaces
//       3 integer part,
//       4 decimal point,
//       5 fraction part,
//       6 'E' or 'e' or 'D' or 'd', exponent marker,
//       7 exponent sign,
//       8 exponent integer part,
//       9 exponent decimal point,
//      10 exponent fraction part,
//      11 blanks,
//      12 final comma or semicolon.
//
//    with most quantities optional.
//
//  Examples:
//
//    S                 R
//
//    '1'               1.0
//    '     1   '       1.0
//    '1A'              1.0
//    '12,34,56'        12.0
//    '  34 7'          34.0
//    '-1E2ABCD'        -100.0
//    '-1X2ABCD'        -1.0
//    ' 2E-1'           0.2
//    '23.45'           23.45
//    '-4.2E+2'         -420.0
//    '17d2'            1700.0
//    '-14e-2'         -0.14
//    'e2'              100.0
//    '-12.73e-9.23'   -12.73 * 10.0**(-9.23)
//
//  Modified:
//
//    07 August 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char *S, the string containing the
//    data to be read.  Reading will begin at position 1 and
//    terminate at the end of the string, or when no more
//    characters can be read to form a legal real.  Blanks,
//    commas, or other nonnumeric data will, in particular,
//    cause the conversion to halt.
//
//    Output, int *LCHAR, the number of characters read from
//    the string to form the number, including any terminating
//    characters such as a trailing comma or blanks.
//
//    Output, bool *ERROR, is true if an error occurred.
//
//    Output, double S_TO_D, the real value that was read from the string.
//
{
  char c;
  int ihave;
  int isgn;
  int iterm;
  int jbot;
  int jsgn;
  int jtop;
  int nchar;
  int ndig;
  double r;
  double rbot;
  double rexp;
  double rtop;
  char TAB ='\t';

  nchar = s_len_trim ( s );
  *error = false;
  r = 0.0E+00;
  *lchar = -1;
  isgn = 1;
  rtop = 0.0E+00;
  rbot = 1.0E+00;
  jsgn = 1;
  jtop = 0;
  jbot = 1;
  ihave = 1;
  iterm = 0;

  for ( ; ; )
  {
    c = s[*lchar+1];
    *lchar = *lchar + 1;
//
//  Blank or TAB character.
//
    if ( c == ' ' || c == TAB )
    {
      if ( ihave == 2 )
      {
      }
      else if ( ihave == 6 || ihave == 7 )
      {
        iterm = 1;
      }
      else if ( 1 < ihave )
      {
        ihave = 11;
      }
    }
//
//  Comma.
//
    else if ( c == ',' || c == ';' )
    {
      if ( ihave != 1 )
      {
        iterm = 1;
        ihave = 12;
        *lchar = *lchar + 1;
      }
    }
//
//  Minus sign.
//
    else if ( c == '-' )
    {
      if ( ihave == 1 )
      {
        ihave = 2;
        isgn = -1;
      }
      else if ( ihave == 6 )
      {
        ihave = 7;
        jsgn = -1;
      }
      else
      {
        iterm = 1;
      }
    }
//
//  Plus sign.
//
    else if ( c == '+' )
    {
      if ( ihave == 1 )
      {
        ihave = 2;
      }
      else if ( ihave == 6 )
      {
        ihave = 7;
      }
      else
      {
        iterm = 1;
      }
    }
//
//  Decimal point.
//
    else if ( c == '.' )
    {
      if ( ihave < 4 )
      {
        ihave = 4;
      }
      else if ( 6 <= ihave && ihave <= 8 )
      {
        ihave = 9;
      }
      else
      {
        iterm = 1;
      }
    }
//
//  Exponent marker.
//
    else if ( ch_eqi ( c, 'E' ) || ch_eqi ( c, 'D' ) )
    {
      if ( ihave < 6 )
      {
        ihave = 6;
      }
      else
      {
        iterm = 1;
      }
    }
//
//  Digit.
//
    else if ( ihave < 11 && '0' <= c && c <= '9' )
    {
      if ( ihave <= 2 )
      {
        ihave = 3;
      }
      else if ( ihave == 4 )
      {
        ihave = 5;
      }
      else if ( ihave == 6 || ihave == 7 )
      {
        ihave = 8;
      }
      else if ( ihave == 9 )
      {
        ihave = 10;
      }

      ndig = char_to_digit ( c );

      if ( ihave == 3 )
      {
        rtop = 10.0E+00 * rtop + ( double ) ndig;
      }
      else if ( ihave == 5 )
      {
        rtop = 10.0E+00 * rtop + ( double ) ndig;
        rbot = 10.0E+00 * rbot;
      }
      else if ( ihave == 8 )
      {
        jtop = 10 * jtop + ndig;
      }
      else if ( ihave == 10 )
      {
        jtop = 10 * jtop + ndig;
        jbot = 10 * jbot;
      }

    }
//
//  Anything else is regarded as a terminator.
//
    else
    {
      iterm = 1;
    }
//
//  If we haven't seen a terminator, and we haven't examined the
//  entire string, go get the next character.
//
    if ( iterm == 1 || nchar <= *lchar + 1 )
    {
      break;
    }

  }
//
//  If we haven't seen a terminator, and we have examined the
//  entire string, then we're done, and LCHAR is equal to NCHAR.
//
  if ( iterm != 1 && (*lchar) + 1 == nchar )
  {
    *lchar = nchar;
  }
//
//  Number seems to have terminated.  Have we got a legal number?
//  Not if we terminated in states 1, 2, 6 or 7!
//
  if ( ihave == 1 || ihave == 2 || ihave == 6 || ihave == 7 )
  {
    *error = true;
    return r;
  }
//
//  Number seems OK.  Form it.
//
  if ( jtop == 0 )
  {
    rexp = 1.0E+00;
  }
  else
  {
    if ( jbot == 1 )
    {
      rexp = pow ( 10.0E+00, jsgn * jtop );
    }
    else
    {
      rexp = jsgn * jtop;
      rexp = rexp / jbot;
      rexp = pow ( 10.0E+00, rexp );
    }

  }

  r = isgn * rexp * rtop / rbot;

  return r;
}
//******************************************************************************



string ktable::table_read(char  *input_filename)

//******************************************************************************
//
//  Purpose:
//
//    DTABLE_READ reads information from a real TABLE file.
//
//  Discussion:
//
//    The file is assumed to contain one record per line.
//
//    Records beginning with the '#' character are comments, and are ignored.
//    Blank lines are also ignored.
//
//    Each line that is not ignored is assumed to contain exactly (or at least)
//    M real numbers, representing the coordinates of a point.
//
//    There are assumed to be exactly (or at least) N such records.
//
//  Modified:
//
//    04 June 2004
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char *INPUT_FILENAME, the name of the input file.
//
//    Output, int *M, the number of spatial dimensions.
//
//    Output, int *N, the number of points.  The program
//    will stop reading data once N values have been read.
//
//    Output, double DTABLE_READ[M*N], the table data.
//
{
  string  error;
  error=table_file_read(input_filename);
  if (error!="") return error;
    
    //error=table_data_read();
    //cerr<< "\n"<<get_options('a')<<"\n"; 
   if (get_options('s')!=0) 
     {
       //cerr<<"Hall"<<get_options('s');
       error=table_sort(get_options('s')-1);
       if (error!="") return error;
 
     }
   if (get_options('a')!=0) 
     {
       row_average(this,get_options('a'));
       //cerr<<"uu";
     }
   return error;
    
}
//******************************************************************************

unsigned int ktable::row_count()

{
  return *n;
}


unsigned int ktable::column_count()

{
  return *m;
}


int ktable::s_len_trim (  string s )

//******************************************************************************
//
//  Purpose:
//
//    S_LEN_TRIM returns the length of a string to the last nonblank.
//
//  Modified:
//
//    26 April 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char *S, a pointer to a string.
//
//    Output, int S_LEN_TRIM, the length of the string to the last nonblank.
//    If S_LEN_TRIM is 0, then the string is entirely blank.
//
{
int n;
n= s.find(' ');

  return n;
}




int mystring::word_count ( string s )

//******************************************************************************
//
//  Purpose:
//
//    WORD_COUNT counts the number of "words" in a string.
//
//  Modified:
//
//    08 February 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char *S, the string to be examined.
//
//    Output, int WORD_COUNT, the number of "words" in the string.
//    Words are presumed to be separated by one or more blanks.
//
{
  bool blank;
  unsigned int j=0;
  int nword;

  nword = 0;
  blank = true;

  while ( j<s.size() ) 
  {
    if ( s[j] == ' ' || s[j]=='\t' )
    {
      blank = true;
    }
    else if ( blank )
    {
      nword = nword + 1;
      blank = false;
    }
    j++;
  }
  if (blank) {j--;}
  //cout <<"nword "<<nword<< " s " <<s<<"\n";
  return nword;
}

bool ktable::ch_eqi ( char c1, char c2 )

//*******************************************************************************
//
//  Purpose:
//
//    CH_EQI is true if two characters are equal, disregarding case.
//
//  Modified:
//
//    13 June 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, char C1, char C2, the characters to compare.
//
//    Output, bool CH_EQI, is true if the two characters are equal,
//    disregarding case.
//
{
  if ( 97 <= c1 && c1 <= 122 ) 
  {
    c1 = c1 - 32;
  } 
  if ( 97 <= c2 && c2 <= 122 ) 
  {
    c2 = c2 - 32;
  }     

  return ( c1 == c2 );
}
//******************************************************************************

int ktable::char_to_digit ( char char_in )

//******************************************************************************

{
  int int_digit=0;

  if ( '0' <= char_in && char_in <= '9' )
  {
    int_digit = char_in - '0';
  }
  else
  {
    int_digit = -1;
  }

  return int_digit;
}
//******************************************************************************

string ktable::table_print(string header,string file)

{
  unsigned int i,j;
  ofstream out;
  string error;
  if (file!="")
    {
      out.open(file.c_str());
      if (!out) 
        {
          error= "Error while opening File: "+file;
          exit(-1);
          return error;
        }
    }
  //cout <<"size:"<<table.size();
  if (!table.empty())
  {
  
    if (get_options('s')!=0) 
     {
       //cerr<<"Hallhall"<<get_options('s')-1;
       error=table_sort(get_options('s')-1);
       if (error!="") return error;
     }
   if (get_options('a')!=0) 
     {
       row_average(this,get_options('a'));
       //cerr<<"uu";
     }
      //cout <<"mm = "<<*m<<"\n";
      //cout <<"nn = "<<*n<<"\n";
  if (file=="")  
    cout <<header;
  else
    out<<header;
  for (i=0;i<(*n);i++)
    {
      if (column_count()>1)
        {
          for (j=0;j<column_count()-1;j++)
            {
              if (file=="")
                {   
                   cout.setf(ios::scientific);
                   cout.precision(get_options('p'));
                   cout<<table[i][j]<<"\t";
                }
              else  
                {   
                   out.setf(ios::scientific);
                   out.precision(get_options('p'));
                   out<<table[i][j]<<"\t";
                }
                
                
           }
          if (file=="")
            {  
              cout.setf(ios::scientific);
              cout.precision(get_options('p'));
              cout<<table[i][j];
            }
          else
            {  
              out.setf(ios::scientific);
              out.precision(get_options('p'));
              out<<table[i][j];
            }
        
        }
      else
        {
          if (file=="")
            {
              cout.setf(ios::scientific);
              cout.precision(get_options('p'));
              cout<<table[i][0];
            }
          else 
            {
              out.setf(ios::scientific);
              out.precision(get_options('p'));
              out<<table[i][0];
            }
          
        }
        
      if (get_options('l')!=0) 
        {
          if (file=="") 
            cout <<"\r"; 
          else 
            out <<"\r"; 
        }
        if (file=="") 
          cout<<"\n";
        else
          out<<"\n";
       
   }  
  } 
  if (file!="") out.close();
  return "";
}
 
 
string  ktable::two_columns(unsigned int j,unsigned int k, ktable *input)
{
unsigned int i;
      //cout<<"j " <<j <<"k "<<k<<"\t";
  table.clear();
  table.resize(input->row_count());
  for (i=0;i<input->row_count();i++)
  {
    table[i].resize(2);
  }
  //cout <<
  if ((j)>=input->column_count() ||(k)>=input->column_count()||j<0 ||k<0) return "row doesn't exist\n";
  
  
  //  cout <<"dimension " << input->row_count()*2;
  for (i=0;i<input->row_count();i++) 
  { 
    table[i][0]=input->table[i][j];
  //  cout <<"value: "<<i*input->column_count()+j <<" "<<input->table[i*input->column_count()+j]<<"\n";
  }
  for (i=0;i<input->row_count();i++) 
  {
    table[i][1]=input->table[i][k];
  }
*m=2;
*n=input->row_count();
return "";
}


string ktable::factor(ktable *input, double factor, unsigned int row, char doperator)
{
  //cerr<<"factor"<<factor<<"\n";
  unsigned int i;
  if (input!=this)
  { 
    //cout <<"poipui";exit(-1);
    table_clear();
    for (i=0;i<(input->row_count());i++)
    {
      add_row(input->table[i]);
    }
  }
  if (row>=(input->column_count())||row<0) return("Factor: Invalid row\n");
  for(i=0;i<input->row_count();i++)
  {
    table[i][row]=op(input->table[i][row],factor,doperator);
  }
  return "";
}



  
string ktable::table_sort(unsigned int column)
 
 {
 if (column<0||column>=column_count()) return("Sort: Invalid row\n");
 if  (get_options('r')) 
                        sort(table.begin(),table.end(),comp_greater(column));
else                    sort(table.begin(),table.end(),comp_lower(column));
 return"";
 }

string ktable::table_paste(ktable *input1, ktable *input2)
{
 table_clear();
 unsigned int i,j;
 vector<double> row;
 for (i=0;i<input1->row_count();i++)
  {
    add_row(input1->table[i]);
  }
 if (input1->row_count()<input2->row_count())
   {
     row.resize(input1->column_count());
     for(i=0;i<input1->column_count();i++)
       {
       row[i]=0;
       }
     for(i= input1->row_count();i<input2->row_count();i++)
      {
        add_row(row);
      }
   }  
     
//  if (input1->row_count()>=input2->row_count())
  //  {
    //  *n=input1->row_count();
     // this->table.resize(input1->row_count());
    //}
  for (i=0;i<input2->row_count();i++)
    {
      for(j=0;j<input2->column_count();j++)
        {
          table[i].push_back(input2->table[i][j]);
        }
    }
     *m= input1->column_count()+   input2->column_count();
        
  if (input1->row_count()>=input2->row_count())
    {
       for(i= input2->row_count();i<input1->row_count();i++)
         {
           for(j=0;j<input2->column_count();j++)
             {
               table[i].push_back(0);
             }
         }
    }
return"";
}


string ktable::select_columns(ktable *input,const vector<unsigned int> &columns)
{
  unsigned int i,j;
  table_clear();
   this->table.resize(input->row_count());
//  vector<double> new_column;
  for (i=0;i<columns.size();i++)
    { 
    //cerr<<input->row_count()<<"\n";
    //cerr<<i<<"\t"<<columns[i]<<"\n";
    
      if(input->column_count()<=columns[i]||columns[i]<0) return ("Row does not exist\n");
        //new_column.clear();
        for (j=0;j<input->row_count();j++)
          {
            //cerr<<j<<"s\t";
            //this->table[j].push_back(input->table[columns[i]][j]);
           this->table[j].resize(i+1);
           this->table[j][i]=input->table[j][columns[i]];

          }
    }
*m=columns.size();
*n=input->row_count();
return "";
}

string ktable::average_two(ktable *input)
{
  ktable in=*input;
  unsigned int i,j;
  vector<double> row;
  row.resize(in.column_count());
  table_clear();
  if (in.row_count()==0) return"";
  if (in.row_count()==1)
    {
      add_row(in.table[0]);
      return"";
    }
  for (i=0;i<in.row_count()-1;i++)
    {
      //cout<< "jj"<<i<<" "<<*n<<"\n";
      for (j=0;j<in.column_count();j++)
        {
          row[j]=(in.table[i][j]+in.table[i+1][j])/2;
        }
      add_row(row);
    }    
*m=in.column_count();
return"";
}


 
void ktable::add_row(const vector<double> &row)
 {
   unsigned int j,k;
     //cout << this->row_count();
     //exit(-1);
     
   if(this->row_count()==0) 
     {
       table.push_back(row);
       *n=1;
       *m=row.size();
     return;
     }
   if(this->column_count()<row.size())
     {
       for (k=0;k<*n;k++)
         {
           if(table[k].size()<row.size())
             {
               j=table[k].size();
               table[k].resize(row.size());
               for(;j<row.size();j++)
                 {
                   table[k][j]=0;
                 }
              }
         }
      *m=row.size();
      }
      
   table.push_back(row);
     
   if(row.size()<*m)
      {
        table[table.size()-1].resize(*m);
        for(k=row.size();k<*m;k++)
          {
            table[table.size()-1][k]=0;
          }    
       }
   ++*n;

   
  }
         
            
string ktable::data_arith(char *input_filenamea, unsigned int rowa1, unsigned int rowa2, double factora1, double factora2,char *input_filenameb,unsigned int rowb1, unsigned int rowb2, double factorb1, double factorb2, char doperator)
 
 {
 cerr<<"dataarith"<<factora1<<factora2<<factorb1<<factorb2<<"\n";
 vector <double> new_row(2);
 ktable ina, inb, tablea,tableb;
 string error;
 int i=0;
 table_clear();
 error=ina.table_read(input_filenamea);
 if (error!="") return error;
 header=ina.table_get_header("all");
 error=tablea.two_columns(rowa1,rowa2,&ina);
 if (error!="") return error;
 tablea.factor(&tablea,factora1,0,get_options('1'));
 tablea.factor(&tablea,factora2,1,get_options('2'));
 error=inb.table_read(input_filenameb);
  if (error!="") return error;
  header=header+inb.table_get_header("all");
 error=tableb.two_columns(rowb1,rowb2,&inb);
 if (error!="") return error;
 tableb.factor(&tableb,factorb1,0,get_options('3'));
 tableb.factor(&tableb,factorb2,1,get_options('4'));
 vector<vector <double> >::iterator listlaufa;
 vector<vector <double> >::iterator listlaufb;
 
 if (tablea.column_count()!=2||tableb.column_count()!=2 )return "Error in data_arith: No 2-Columm table\n";
 tablea.table_sort(0);
 tableb.table_sort(0);
 listlaufa=tablea.table.begin();
 listlaufb=tableb.table.begin();
// cout <<"\n";
 do
 {
  i++;
                  

   while ((listlaufb!=tableb.table.end()))
     {
      if (((listlaufb+1)==tableb.table.end())) break;
       if ((*(listlaufb+1))[0]<((*listlaufa)[0])) listlaufb++; else break;
     
       }
   //cerr<<"laufa: "<<(*listlaufa)[0]<<" laufb: "<<(*listlaufb)[0]<<"\n";
               

   if (listlaufb!=tableb.table.end()&&(listlaufb+1)!=tableb.table.end()) 
     {
       if ((*(listlaufa))[0]>=(*tableb.table.begin())[0])  //1=steigxvalue                 <svalue>
         {
        new_row[0]=(*listlaufa)[0];
        new_row[1]=op(((*listlaufa)[1]),
        ((*listlaufb)[1]+(((*(listlaufb+1))[1]-(*(listlaufb))[1])/((*(listlaufb+1))[0]-(*(listlaufb))[0]))
        *((*listlaufa)[0]-(*listlaufb)[0])),doperator);
        add_row(new_row);
          //cout<<column_count();  
          //cout<<row_count(); 
        // cout <<"size "<<table.size()<<"\n";
         //table.resize(table.size()+1);
         //cout <<"sizeafter "<<table.size()<<"\n";
         //table[table.size()-1].resize(2);
         //table[table.size()-1][0]=new_row[0];
         //table[table.size()-1][1]=new_row[1];
         
         //print_table();
         }
       }

       listlaufa++;
       
     //cerr <<"i "<<i<<"\n";
     }
while (listlaufa!=tablea.table.end());


 //cout <<"\n";
return"";
 }
double ktable::op(double in1, double in2, char op_in)

{
switch (op_in) 
  {case '+': return in1+in2;
   case '-': return in1-in2;
   case '*': return in1*in2;
   case '/': return in1/in2;
   case 's': return in2; //for factor set
   case 'i': return 1/in1; //for factor set
   
   //case '^': return in1in2;
   default: return -1;
  }
}
void ktable::table_clear()
{
table.clear();
*n=0;*m=0;
}

string ktable::integral(ktable *input)
{
  if(input->column_count()!=2) return("Integral_2Col: Input Table should contain 2 columns");
  double integral=0,xa,xb,ya,yb;
  ktable in=*input;
  in.table_sort(0);
      //in.print_table("");exit(-1);
  table_clear();
  vector <double> new_row(2);
  vector<vector <double> >::iterator iter_in;
  iter_in=in.table.begin();
  new_row[0]=(*iter_in)[0];
  xa=new_row[0];
  new_row[1]=0; 
  ya=(*iter_in)[1]; 
  
  add_row(new_row);
  do
    {
      yb=ya;
      xb=xa;
      iter_in++;
      if (iter_in==in.table.end())break;
      xa=(*iter_in)[0];
      ya=(*iter_in)[1]; 
      integral=integral+(yb+ya)/2*(xa-xb);
      new_row[0]=xa;
      new_row[1]=integral;
      add_row(new_row);
    }
  while(iter_in!=in.table.end());

  return"";
}



string ktable::derivative(ktable *input)
{
  if(input->column_count()!=2) return("derivative_2Col: Input Table should contain 2 columns\n");
  if(input->row_count()<2) return("derivative_2Col: Input Table should contain at least 2 columns\n");

  double xa,xb,ya,yb;
  
  ktable in=*input;
    in.table_sort(0);
  table_clear();
    vector <double> new_row(2);
  vector<vector <double> >::iterator iter_in;
  iter_in=in.table.begin();
  //cerr<<(*iter_in)[0]<<"\t"<<(*iter_in)[1]<<"\n";
  xa=(*iter_in)[0];
  ya=(*iter_in)[1]; 
  
  do
    {
      yb=ya;
      xb=xa;
      iter_in++;
      if (iter_in==in.table.end())break;
      xa=(*iter_in)[0];
      ya=(*iter_in)[1]; 
      new_row[0]=(xa+xb)/2;
      if (abs(xb-xa)>1e-20)
        {
          new_row[1]=(yb-ya)/(xb-xa);
        }
      else
        {
          new_row[1]=0;
        }
      add_row(new_row);
    }
  while(iter_in!=in.table.end());

  return"";
}

string ktable::dummy(ktable *input)
{
 vector <double> new_row(2);
 for (unsigned int i=0;i<input->row_count();i++)
   {
     new_row=input->table[i];
     add_row(new_row);
   }

  return"";
}



string ktable::calc_xyf(ktable *input, unsigned int x, unsigned int y, double x_factor, double y_factor,string(ktable::*calc_func)(ktable *input))
{
  string error;
  table_clear();
  ktable in_two;
  error=in_two.two_columns(x,y,input);

  if (error!="") return error;

  error=in_two.factor(&in_two,x_factor,0,get_options('1'));
  if (error!="") return error;
  error=in_two.factor(&in_two,y_factor,1,get_options('2'));
  if (error!="") return error;
  //error=in_two.factor(&in_two,y_factor,1);
  //if (error!="") return error;
  error=(*this.*calc_func)(&in_two);
  if (error!="") return error;
  return "";


}



string ktable::row_average(ktable *input, unsigned int number_of_rows)
{
  ktable output;
  unsigned int i,j,k;
  output.table_clear();
  if (number_of_rows<1) return "number of rows for average should be >0\n";
  vector <double> new_row(input->column_count());
  for (i=0; i<input->row_count() /  number_of_rows;i++)
    {
     // cerr<<"i: "<<i<<"\n";
      for (j=0;j<input->column_count();j++)
        {
          new_row[j]=0;
          for (k=0;k<number_of_rows;k++)
            {
              new_row[j]+=input->table[i*number_of_rows+k][j];
              //cerr<<"new_row: ["<<i<<"]["<<j<<"]: "<<new_row[j]<<"\n";
            }
          new_row[j]= new_row[j]/number_of_rows;
       //   cerr <<"av:["<<j<<"]:"<<new_row[j]<<"\n";
        }  
        output.add_row(new_row);

    }       
     //cerr <<"Hh"<<input->row_count()  % number_of_rows<<"\n";
     if ((input->row_count()  % number_of_rows)!=0)
      {
       for (j=0;j<input->column_count();j++)
        {
           new_row[j]=0;
           for (k=0;k<(input->row_count()  % number_of_rows);k++)
             {
               new_row[j]+=input->table[(input->row_count() /  number_of_rows*number_of_rows+k)][j];
               //cerr<<input->row_count()  % number_of_rows<<"\n";
               //cerr<<"new_row: ["<<k<<"]["<<j<<"]: "<<new_row[j]<<"\n";
             }
           new_row[j]= new_row[j]/(input->row_count()  % number_of_rows);
        } 
          //cerr <<"lala";
      output.add_row(new_row);
      }
      table_clear();
      for (i=0;i<output.row_count();i++)
        {
          add_row(output.table[i]);
        }
return "";
}      


string ktable::linearregression(ktable *input, unsigned int x,unsigned int y, double x_factor,double y_factor, unsigned int number_of_rows)
{
  vector <double> new_row(2);
  table_clear();
  string error;
  ktable in_two;
  double average;
  unsigned int i,j;
  double xin[number_of_rows];
  double yin[number_of_rows];
  Linear lin;
  error=in_two.two_columns(x,y,input);
  if (error!="") return error;
  in_two.table_sort(0);
  error=in_two.factor(&in_two,x_factor,0,get_options('1'));
  if (error!="") return error;
  error=in_two.factor(&in_two,y_factor,0,get_options('2'));
  if (error!="") return error;
  if (number_of_rows<3) return "number of rows for linear should be >2\n";
  for (i=0; i<input->row_count() /  number_of_rows;i++)
   {
     average=0;
     for(j=0;j<number_of_rows;j++)
       {
         xin[j]=in_two.table[j+i*number_of_rows][0];
         //cerr<<"jj"<<j+i*number_of_rows<<"\t"<<in_two.table[j+i*number_of_rows][0]<<"\n";
         average+=xin[j];
         yin[j]=in_two.table[j+i*number_of_rows][1];
         //cerr<<yin[j]<<"\n";;
         
       }  
     average/=number_of_rows;
     if (lin.set(number_of_rows,xin,yin))
       {  
          new_row[0]=average;
          new_row[1]=lin.getSlope();
          add_row(new_row);
       }
     //cerr <<new_row[0]<<"\t"<<new_row[1]<<"\n";  
     //cerr<<"i"<<i<<"\n";   
   }
    if ((input->row_count()  % number_of_rows)!=0)
    {
       average=0;
       for(j=0;j<input->row_count() % number_of_rows;j++)
       //cerr<<input->row_count() % number_of_rows<<j+input->row_count() /  number_of_rows<<"\n";
       {
         xin[j]=in_two.table[j+input->row_count()/number_of_rows*number_of_rows][0];
         //cerr<<"jj"<<j+i*number_of_rows<<"\t"<<in_two.table[j+i*number_of_rows][0]<<"\n";
         average+=xin[j];
                  yin[j]=in_two.table[j+input->row_count()/number_of_rows*number_of_rows][1];
         //cerr<<yin[j]<<"\n";;
         
       }  
       //cerr<<j<<" "<<  input->row_count() % number_of_rows<<"\n";exit(-1);
       average/=input->row_count() % number_of_rows;
       if (lin.set(input->row_count() % number_of_rows,xin,yin))
       {
         new_row[0]=average;
         new_row[1]=lin.getSlope();
         add_row(new_row);
       }
     }
   //exit(-1);
       
  return "";
}
           
string ktable::row_select(ktable *input, unsigned int column, double value, double derivation)
{
  unsigned int i;
  //cerr<<column<<"\t"<<value<<"\t"<<derivation;exit(-1);
  ktable output;
  output.table_clear();
  if (column<0||column>=input->column_count()) return "Select: Invalid Column number\n";
  for (i=0;i<input->row_count();i++)
  {
           //cerr <<abs(input->table[i][column]-value)<<"\n";
    if (abs(input->table[i][column]-value)<derivation)
      {
        output.add_row(input->table[i]);
      }
  }
  table_clear();
  for (i=0;i<output.row_count();i++)
        {
          add_row(output.table[i]);
        }
return "";
}     

string ktable::table_get_header(string what)
{
  if (what=="all")
  return header;
  else if (what=="firstline")
  return get_first_line(header);
  else if (what=="withoutfirstline")
  return (remove_first_line(header));
  else if (what=="firstlinewithout#")
  return '\t'+ (remove_first_raute(get_first_line(header)));
  else return header;
  
};


string ktable::line_end()
{
  if (get_options('l')==false) return "\n"; else return "\r\n";
}

    
