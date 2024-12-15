# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <string>
using namespace std;
# include "ktable.h"
# include <getopt.h>
# include <string.h>
# include "linear.h"
# include "version.h"


void error_handle(string error)
{
  if (error=="") return;
  cerr<<"tab: "<<error<<"Type: tab -h for more info\n";
  exit(-1);
}

void print_usage()
{
  cout<<"tab does some simple 2D table manipulations\n\n"
      <<"Usage: tab [OPTION] METHOD [PARAMETER]\n\n"
      <<"Options:\n"
      <<"  -s COLUMN , --pre_sort      Sorts inputfile according to column COLUMN\n"
      <<"  -S COLUMN , --post_sort      Sorts outfile according to column COLUMN\n"
      <<"  -a ROW , --pre_average      Averages inputfile over ROW rows \n"
      <<"  -A ROW , --post_average      Averages outfile over  ROW rows \n"
      <<"  -r , --reverse      Sort in descending order\n"
      <<"  -l +/-, --linewindows      Windows-like Carriage Linefeed on / of\n\n"
      <<"  -H w/o/a/n, --header      Write Header (write) / write original Header (original) / write all headers (all)/write no header (no)\n\n"
      <<"  -o file, --outfile          Write to file instead of standard output\n\n"
      <<"  -P +/-/columnname  --append       Appends result for integral/derivative in an extra row/ give row a name /turn it off\n\n"
      <<"  -F {x,y}{1,2}{+,-*,/,s,i},  set operation for factor option (for integration /derivative method), default is '*'  (Example: -F x1+)\n\n"
      <<"  -p NUMBER, --precision      number of printed digits after the decimal separator (Default 8)\n\n"
      <<"      --help        display this help and exit\n"
      <<"      --version     output version information and exit\n\n"
      <<"Methods:\n\n"
      <<"CALC:\n\n tab [-,+,/,'*']  file1.dat COLUMN_X1 COLUMN_Y1 FAKTOR_Y1 file1.dat COLUMN_X2 COLUMN_Y2 FAKTOR_Y2 \n\n"
      <<" tab [-,+,/,'*']  file1.dat COLUMN_X1 COLUMN_Y1  file1.dat COLUMN_X2 COLUMN_Y2 \n                (FAKTOR_Y1=FAKTOR_Y2=1) \n"
      <<" tab [-,+,/,'*']  file1.dat COLUMN_Y1 file1.dat COLUMN_Y2\n                 (COLUMN_X1=COLUMN_X2=1; FAKTOR_Y1=FAKTOR_Y2=1)\n"
      <<" tab [-,+,/,'*']  file1.dat  file2.dat\n                  (COLUMN_X1=COLUMN_X2=1;COLUMN_Y1=COLUMN_Y2=2; FAKTOR_Y1=FAKTOR_Y2=1)\n\n\n"
      <<"[-,+,/,'*'] is equivalent to [difference,sum,product,quotient]\n\n"
      <<"Calculation with linear interpolation\n\n"
      <<"Example: 'tab - file1.dat 1 3 file2.dat 1 3' takes differences between Column 3 in file2.dat and Column 3 in file1.dat refering to colukn as x value, respectivly\n\n\n"
      <<"INTEGRATE/DERIVATE/CUT 2 Columns:\n\n tab [i,d,D]  file.dat  COLUMN_X COLUMN_Y FAKTOR_X FAKTOR_Y\n"
      <<" tab [i,d,D]  file.dat  COLUMN_X COLUMN_Y FAKTOR_Y\n           (FAKTOR_X=1)\n"
      <<" tab [i,d,D]  file.dat  COLUMN_X COLUMN_Y\n           (FAKTOR_X=1;FAKTOR_Y=1)\n"
      <<" tab [i,d,D]  file.dat  COLUMN_Y\n           (COLUMN_X=1;FAKTOR_X=1;FAKTOR_Y=1)\n"
      <<" tab [i,d,D]  file.dat\n           (COLUMN_X=1;COLUMN_Y=2;FAKTOR_X=1;FAKTOR_Y=1)\n\n"
      <<"[i,d,D] is equivalent to [integral,derivative,cut2col]"
      <<"Integration/Derivation/Selection of two columns\n\n\n"
      <<"SORT:\n\n tab s (sort) file.dat COLUMN\n\n"
      <<" Sorts the file arcording to column COLUMN\n\n\n"
      <<"FACTOR:\n\n\n tab f (factor) file.dat COLUMN FACTOR [+,-,'*',/,s,i]\n\n"
      <<" changes column COLUMN by adding/substracting/multipying/dividing/setting fixed the factor FAKTOR; i: column=1/column\n\n"
      <<" Example: 'tab f file.dat 2 3 +' adds 3 to every value of the second column\n\n" 
      <<"AVERAGE\n\n tab a (average) file.dat NUMBER_OF_ROWS\n\n\n"
      <<"LINEAR\n\n tab l (linear) file.dat COLUMN_X1 COLUMN_X2 NUMBER_REG\n\n\n"
      <<"SELECT\n\n tab e (select) file.dat COLUMN VALUE [DERIVATION]\n\n"
      <<" Selects only the rows, where the column COLUMN has the VALUE values. The preset derivation is 1e-20\n\n\n"
      <<"CUT\n\n tab c (cut) file.dat ROW1[ROW2 ..ROWN]\n\n"
      <<" cuts the rows ROW1 to ROWN from the input file, in the order ROW1 to ROWN.\n\n\n"
      <<"PASTE\n\n tab P (paste) file1.dat file2.dat\n\n"
      <<" pastes the to inputfiles sequentially row by row. Values which not exist (Different # of rows) are set 0.\n\n\n"
      <<"PRINT\n\n tab p file.dat\n\n"
      <<" Just prints the input file (For test purposes)\n\n\n"
      <<"Report bugs to  berggold@ph2.uni-koeln.de\n";

      exit(-1);
}




// **************************************************************
//  string get_header(int argc, char **argv)
//
//  Puts all argv[] arguments in a string
// **************************************************************

string get_header(int argc, char **argv)
  {
          int i;  
          string header="#";
          for (i=0;i<argc-1;i++) header=header+argv[i]+" ";
          header=header+argv[i];
          return header;
  }

// **************************************************************

bool valid_method(char input_method)
{
  switch (input_method)
    {
      case '+':case '-':case '*':case '/':case 'i':case 's':case 'f':case 'a':case 'd':case 'l':case 'p':case 'e':case 'O':case 'D':case 'c':case 'P':
      return true;
      default: return false;
    }
}

char short_method(char *method)
{
  char method_nr='?';
  if(strcmp(method,"difference")==0||strcmp(method,"-")==0) method_nr='-';
  if(strcmp(method,"sum")==0||strcmp(method,"+")==0) method_nr='+';
  if(strcmp(method,"product")==0||strcmp(method,"*")==0) method_nr='*';
  if(strcmp(method,"quotient")==0||strcmp(method,"/")==0) method_nr='/';
  if(strcmp(method,"integral")==0||strcmp(method,"i")==0) method_nr='i';
  if(strcmp(method,"derivative")==0||strcmp(method,"d")==0) method_nr='d';
  if(strcmp(method,"factor")==0||strcmp(method,"f")==0) method_nr='f';
  if(strcmp(method,"sort")==0||strcmp(method,"s")==0) method_nr='s';
  if(strcmp(method,"test")==0||strcmp(method ,"t")==0) method_nr='t';
  if(strcmp(method,"average")==0||strcmp(method,"a")==0) method_nr='a';
  if(strcmp(method,"print")==0||strcmp(method,"p")==0) method_nr='p';
  if(strcmp(method,"select")==0||strcmp(method,"e")==0) method_nr='e';
  if(strcmp(method,"cut2col")==0||strcmp(method,"D")==0) method_nr='D';
  if(strcmp(method,"cut")==0||strcmp(method,"c")==0) method_nr='c';
  if(strcmp(method,"paste")==0||strcmp(method,"P")==0) method_nr='P';
  if(strcmp(method,"linear")==0||strcmp(method,"l")==0) method_nr='l';



  return method_nr;
}


// **************************************************************
//  main (int argc, char **argv)
//  
// **************************************************************



int main (int argc, char **argv)



{
  mystring option;
  unsigned int i=1, col_x1,col_x2,col_y1,col_y2,start_nonopt=0,j,linear;
  ifstream input;
  string o,tempstring;
  double factor_x1,factor_x2,factor_y1,factor_y2,derivation;
  string home=getenv("HOME");
  string config_file,header="";
  config_file= home+"/.tab";
  ktable in,in1,outc;
  char *file_a=NULL,*file_b=NULL, *method;
  char method_nr=0;
   vector<unsigned int> columns;
     
  static struct option long_options[] =
             {
               {"preaverage",     required_argument,       0, 'a'},
               {"postaverage",     required_argument,       0, 'A'},
               {"presort",     required_argument,       0, 's'},
               {"postsort",     required_argument,       0, 'S'},
               {"test",  required_argument,       0, 't'},
               {"help",  no_argument,       0, 'h'},
               {"version",  no_argument,       0, 'v'},
               {"reverse",  no_argument,       0, 'r'},
               {"linewindows",  required_argument,       0, 'l'},
               {"header",  required_argument,       0, 'H'},
               {"precision",  required_argument,       0, 'O'},
               {"outfile",  required_argument,       0, 'o'},
               {"append",  required_argument,       0, 'P'},
               {"factorop",  required_argument,       0, 'F'},
               
               
               
               
               
               {0, 0, 0, 0}
             };
  static char optstr[]="-a:A:hH:vrl:s:p:P:S:F:t:o:0123456789";
  int option_char;
  int option_index = 0;
  string (ktable::*calc_fct)( ktable*);
 
  input.open (config_file.c_str());

  if ( input )
    {
      getline ( input,option);
    }
            //cout <<option.word_count(option);exit(-1);};
            //typedef string (ktable::*ptintegral)(ktable *);

  if (argc==1) 
    {
      print_usage();
    }

  while ((option_char = getopt_long(argc, argv, optstr,long_options,&option_index)) != EOF )
     { 
//     cerr <<option_char<<"\n";
       switch (option_char)
         {
           case 'a':i++;
                    i++;
                    in.set_options('a',atoi(optarg));
                    in1.set_options('a',atoi(optarg));
                    
                    break;
           case 'A':i++;
                    i++;
                    outc.set_options('a',atoi(optarg));
                    break;
           case 's':i++;
                    i++;
                    in.set_options('s',atoi(optarg));
                    in1.set_options('s',atoi(optarg));
                    
                    break;
           case 'S':i++;
                    i++;
                    outc.set_options('s',atoi(optarg));
                    break;
           case 'r':i++;
                    outc.set_options('r',true);
                    in.set_options('r',true);
                    in1.set_options('r',true);
                    break;
           
           case 'F':i++;i++;
                    if (strlen(optarg)!=3) break;
                    if (strncmp(optarg,"x1",2)==0) outc.set_options('1', optarg[2]);
                    if (strncmp(optarg,"y1",2)==0) outc.set_options('2', optarg[2]);
                    if (strncmp(optarg,"x2",2)==0) outc.set_options('3', optarg[2]);
                    if (strncmp(optarg,"y2",2)==0) outc.set_options('4', optarg[2]);
                    
                    break;
           
           case 'l':i++;i++;
                    if (*optarg=='+')
                    {
                      outc.set_options('l',true);
                      in.set_options('l',true);
                      in1.set_options('l',true);
                    
                    }  
                    else 
                    {
                      outc.set_options('l',false);
                      in.set_options('l',false);
                      in1.set_options('l',true);
                    
                    }
                    break;
           case 'H':i++;i++;
                    if (*optarg=='w')
                    {
                      outc.set_options('H',true);
                      outc.set_options('O',false);
                    }
                    if (*optarg=='o')
                    {
                      outc.set_options('H',false);
                      outc.set_options('O',true);
                    }  
                    if (*optarg=='a')
                    {
                      outc.set_options('H',true);
                      outc.set_options('O',true);
                    }
                    if (*optarg=='n')
                    {
                      outc.set_options('H',false);
                      outc.set_options('O',false);
                    }

                    break;
           case 'p':i++;i++;
                    outc.set_options('p',atoi(optarg));
                    break;
           case 'o':i++;i++;
                    tempstring=optarg;
                    outc.set_options_string('o',tempstring);
                    break;
           case 'P':i++;i++;
                    tempstring=optarg;
                    
                    if (*optarg=='+')
                    {
                      outc.set_options('P',true);
                      outc.set_options_string('P',"");
                    }  
                    else if (*optarg=='-')
                    {
                      outc.set_options('P',false);
                    }
                    else 
                    {
                      outc.set_options('P',true);
                      outc.set_options_string('P',tempstring);
                    }
                    break;
           
           case 'h':print_usage();break;
           case 'v':print_version();break;
            default:if (valid_method(short_method(argv[i]))&&start_nonopt==0) 
                      {
                         start_nonopt=i;break;
                       }
                    else if (start_nonopt==0) error_handle("unknown option / Method\n");
         }
    }            



  method=argv[start_nonopt];
  method_nr=short_method(method);
  //cerr <<"method_nr: " <<method_nr<<"\n";
  //cerr <<"arg " <<argv[0]<<"\t "<<argv[1]<<"\t"<<argv[2]<<"\n";
  //<<"\t "<<argv[3]<<"\t "<<argv[4]<<"\n";
  //<<"\t "<<argv[5]<<"\t"<<argv[6]<<"\t"<<argv[7]<<"\t"<<argv[8]<<"\t"<<argv[9]<<"\n";
  //cerr <<"argc: "<<argc<<"\n"; 
  //cerr<<"nummer "<<atof(argv[start_nonopt+4])<<"\n";
 
switch (method_nr)
    {
      case '/':case '*': case '+': case '-':
        //  cerr <<argc;
                     col_x1    = 0;
                     col_y1    = 1;
                     factor_x1 = 1;
                     factor_y1 = 1;
                     col_x2    = 0;
                     col_y2    = 1;
                     factor_x2 = 1;
                     factor_y2 = 1;

        switch (argc-start_nonopt+1)
          {
            case 12: file_a    = argv[start_nonopt+1];
                     col_x1    = atoi(argv[start_nonopt+2])-1;
                     col_y1    = atoi(argv[start_nonopt+3])-1;
                     factor_x1 = atof(argv[start_nonopt+4]);
                     factor_y1 = atof(argv[start_nonopt+5]);
                     file_b    = argv[start_nonopt+6];
                     col_x2    = atoi(argv[start_nonopt+7])-1;
                     col_y2    = atoi(argv[start_nonopt+8])-1;
                     factor_x2 = atof(argv[start_nonopt+9]);
                     factor_y2 = atof(argv[start_nonopt+10]);
                     break;
            case 10: file_a    = argv[start_nonopt+1];
                     col_x1    = atoi(argv[start_nonopt+2])-1;
                     col_y1    = atoi(argv[start_nonopt+3])-1;
                     factor_y1 = atof(argv[start_nonopt+4]);
                     file_b    = argv[start_nonopt+5];
                     col_x2    = atoi(argv[start_nonopt+6])-1;
                     col_y2    = atoi(argv[start_nonopt+7])-1;
                     factor_y2 = atof(argv[start_nonopt+8]);
                     break;            
            case  8: file_a    = argv[start_nonopt+1];
                     col_x1    = atoi(argv[start_nonopt+2])-1;
                     col_y1    = atoi(argv[start_nonopt+3])-1;
                     file_b    = argv[start_nonopt+4];
                     col_x2    = atoi(argv[start_nonopt+5])-1;
                     col_y2    = atoi(argv[start_nonopt+6])-1;
                     break;
            case  6: file_a    = argv[start_nonopt+1];
                     col_y1    = atoi(argv[start_nonopt+2])-1;
                     file_b    = argv[start_nonopt+3];
                     col_y2    = atoi(argv[start_nonopt+4])-1;
                     break;
            case  4: file_a    = argv[start_nonopt+1];
                     file_b    = argv[start_nonopt+2];
                     break;
            default: error_handle("Wrong parameter list\n");
          }
         //cerr<<"ktable"<<factor_x1<<factor_y1<<factor_x2<<factor_y2<<"\n";
        error_handle(outc.data_arith(file_a,col_x1,col_y1,factor_x1,factor_y1,file_b,col_x2,col_y2,factor_x2,factor_y2,method_nr));
        if (outc.get_options('H')) 
           {
              header="#X\tY"+outc.line_end()+get_header(argc,argv)+outc.line_end();
           }
        if (outc.get_options('O')) 
             header=header+outc.table_get_header("all");
        outc.table_print(header,outc.get_options_string('o'));
        break;

    case 'i':case 'd':case 'D':
      if (method_nr=='i') calc_fct=&ktable::integral;
      if (method_nr=='d') calc_fct=&ktable::derivative;
      if (method_nr=='D') calc_fct=&ktable::dummy;
      
      if (argc-start_nonopt!=1)error_handle(in.table_read(argv[start_nonopt+1]));
      //in.print_table();
      switch (argc-start_nonopt+1)
       {  
         case 3: col_x1=0;
                 col_y1=1;
                 factor_x1=1; 
                 factor_y1=1; 
                 break;
        case 4:  col_x1=0;
                 col_y1=atoi(argv[start_nonopt+2])-1;
                 factor_x1=1;
                 factor_y1=1;
                 break;
         case 5: col_x1=atoi(argv[start_nonopt+2])-1;
                 col_y1=atoi(argv[start_nonopt+3])-1;
                 factor_x1=1;
                 factor_y1=1;
                 break;
         case 6: col_x1=atoi(argv[start_nonopt+2])-1;
                 col_y1=atoi(argv[start_nonopt+3])-1;
                 factor_y1=atof(argv[start_nonopt+4]);
                 factor_x1=1;
                 break;
         case 7: col_x1=atoi(argv[start_nonopt+2])-1;
                 col_y1=atoi(argv[start_nonopt+3])-1;
                 factor_x1=atof(argv[start_nonopt+4]);
                 factor_y1=atof(argv[start_nonopt+5]);
                 
                 break;
       
         default: error_handle("Wrong parameter list\n");
       }
       //cerr<<"\n"<<col_x1<<" "<<col_y1<<" "<<factor_x1<<" "<<factor_y1<<"\n";exit(-1);          
       error_handle(outc.calc_xyf(&in,col_x1,col_y1,factor_x1,factor_y1,calc_fct));
       if (outc.get_options('P'))
         {
            if (method_nr=='d')
              {
                in.average_two(&in);
              }
            if (outc.get_options_string('P')=="")
              {
                  if (method_nr=='i') tempstring="\tintegral";
                  if (method_nr=='d') tempstring="\tderivative";
                  if (method_nr=='D') tempstring="\tdummy";
              }
            else
              tempstring='\t'+outc.get_options_string('P');
            if (outc.get_options('O')) header=in.table_get_header("firstline")+tempstring+outc.line_end()+in.table_get_header("withoutfirstline");
            if (outc.get_options('H')) header=header+get_header(argc,argv)+outc.line_end();
         }
       else
         {      
           if (outc.get_options('H')) header="#X\tY"+outc.line_end()+get_header(argc,argv)+outc.line_end();
           if (outc.get_options('O')) header=header+in.table_get_header("all");
         }
       
       if (outc.get_options('P')) 
         {
           vector<unsigned int> rows;
           rows.push_back(1);
           in1.select_columns(&outc,rows);
           outc.table_paste(&in,&in1);
           //cerr<<outc.column_count()<<" "<<outc.row_count()<<" ";
                if (outc.get_options_string('o')=="")
                  outc.table_print(header,"");
                else
                  outc.table_print(header,outc.get_options_string('o'));
          }
       else outc.table_print(header,outc.get_options_string('o'));
       break;      
    
    case 'f':
      if (argc-start_nonopt!=5) error_handle("Faktor: Wrong parameter list\n");
      error_handle(in.table_read(argv[start_nonopt+1]));
      error_handle(outc.factor(&in,atof(argv[start_nonopt+3]),atoi(argv[start_nonopt+2])-1,*argv[start_nonopt+4]));
      if (outc.get_options('O')) header=in.table_get_header("all");
      if (outc.get_options('H')) header=header+get_header(argc,argv)+outc.line_end();
      error_handle(outc.table_print(header,outc.get_options_string('o')));
      break;

    case 'c':
       if (argc-start_nonopt<3) error_handle("Cut: Wrong parameter list\n");
       for (j=2;j<argc-start_nonopt;j++)
         {
            columns.push_back(atoi(argv[start_nonopt+j])-1);
         }
       error_handle(in.table_read(argv[start_nonopt+1]));
       error_handle(outc.select_columns(&in,columns));
      if (outc.get_options('O')) header=in.table_get_header("all");
      if (outc.get_options('H')) header=header+get_header(argc,argv)+outc.line_end();
      error_handle(outc.table_print(header,outc.get_options_string('o')));
      break;  
      
    case 'P':
       if (argc-start_nonopt!=3) error_handle("Paste: Wrong parameter list\n");
       error_handle(in.table_read(argv[start_nonopt+1]));
       error_handle(in1.table_read(argv[start_nonopt+2]));
       error_handle(outc.table_paste(&in,&in1));
       if (outc.get_options('O')) header=in.table_get_header("firstline")+in1.table_get_header("firstlinewithout#")+outc.line_end()
       +in.table_get_header("withoutfirstline")+in1.table_get_header("withoutfirstline");
       if (outc.get_options('H')) header=header+get_header(argc,argv)+outc.line_end();
       error_handle(outc.table_print(header,outc.get_options_string('o')));
       break;
       
    case 's':
      if (argc-start_nonopt!=3) error_handle("Sort: Wrong parameter list\n");
      error_handle(outc.table_read(argv[start_nonopt+1]));
      error_handle(outc.table_sort(atoi(argv[start_nonopt+2])-1));
      if (outc.get_options('O')) header=outc.table_get_header("all");
      if (outc.get_options('H')) header=header+get_header(argc,argv)+outc.line_end();
      outc.table_print(header,outc.get_options_string('o'));
      break;
      
    case 'a':
      {
      if (argc-start_nonopt!=3) error_handle("Average: Wrong parameter list\n"); 
      error_handle(outc.table_read(argv[start_nonopt+1]));
      //outc.print_table();
      error_handle(outc.row_average(&outc,atoi(argv[start_nonopt+2])));
      if (outc.get_options('O')) header=outc.table_get_header("all");
      if (outc.get_options('H')) header=header+get_header(argc,argv)+outc.line_end();
      outc.table_print(header,outc.get_options_string('o'));
      break;
      }  
    
    case 'l':
      {
       if (argc-start_nonopt<3||argc-start_nonopt>8) error_handle("linear: Wrong parameter list\n"); 
         
       switch (argc-start_nonopt+1)
       {  
         case 4: col_x1=0;
                 col_y1=1;
                 factor_x1=1; 
                 factor_y1=1; 
                 linear=atoi(argv[start_nonopt+2]);
                 break;
        case 5:  col_x1=0;
                 col_y1=atoi(argv[start_nonopt+2])-1;
                 factor_x1=1;
                 factor_y1=1;
                 linear=atoi(argv[start_nonopt+3]);
                 
                 break;
         case 6: col_x1=atoi(argv[start_nonopt+2])-1;
                 col_y1=atoi(argv[start_nonopt+3])-1;
                 factor_x1=1;
                 factor_y1=1;
                 linear=atoi(argv[start_nonopt+4]);
                 
                 break;
         case 7: col_x1=atoi(argv[start_nonopt+2])-1;
                 col_y1=atoi(argv[start_nonopt+3])-1;
                 factor_y1=atof(argv[start_nonopt+4]);
                 factor_x1=1;
                 linear=atoi(argv[start_nonopt+5]);
                 
                 break;
         case 8: col_x1=atoi(argv[start_nonopt+2])-1;
                 col_y1=atoi(argv[start_nonopt+3])-1;
                 factor_x1=atof(argv[start_nonopt+4]);
                 factor_y1=atof(argv[start_nonopt+5]);
                 linear=atoi(argv[start_nonopt+6]);
                 break;
       }
    
      error_handle(in.table_read(argv[start_nonopt+1]));
      //outc.print_table();
      error_handle(outc.linearregression(&in,col_x1,col_y1,factor_x1,factor_y1,linear));
      if (outc.get_options('O')) header=outc.table_get_header("all");
      if (outc.get_options('H')) header=header+get_header(argc,argv)+outc.line_end();
      outc.table_print(header,outc.get_options_string('o'));
      break;
      }  
    
    case 'e':
      {
      if ((argc-start_nonopt!=5)&&(argc-start_nonopt!=4)) error_handle("Average: Wrong parameter list\n");
      if (argc-start_nonopt==4) 
        {
          derivation=1e-20;
        }
          else 
            {
              derivation=atof(argv[start_nonopt+4]);
            }
        error_handle(outc.table_read(argv[start_nonopt+1]));
        error_handle(outc.row_select(&outc,atoi(argv[start_nonopt+2])-1,atof(argv[start_nonopt+3]),derivation ));
        if (outc.get_options('O')) header=outc.table_get_header("all");
      if (outc.get_options('H')) header=header+get_header(argc,argv)+outc.line_end();
        outc.table_print(header,outc.get_options_string('o'));
        break;
      
      }

    case 'p':
      {
      if (argc-start_nonopt!=2) error_handle("Print: Wrong parameter list\n"); 
      error_handle(outc.table_read(argv[start_nonopt+1]));
      if (outc.get_options('O')) header=outc.table_get_header("all");
      if (outc.get_options('H')) header=header+get_header(argc,argv)+outc.line_end();
      outc.table_print(header,outc.get_options_string('o'));
      break;
      
      }  

    default: error_handle("Unknown function\n");
  }

}

