   #include <vector>

class table2d{
public:
vector< vector<double> > table;
};

class comp_lower : public std::binary_function<vector<double>,vector<double>,bool> 
{
   int row;
public:
   comp_lower( const int r ) : row(r) {}
   bool operator()(vector<double> a , vector<double>b ) const
   {
    return (a[row]) < (b[row]) ;
   }
};

class comp_greater : public std::binary_function<vector<double>,vector<double>,bool> 
{
   int row;
public:
   comp_greater( const int r ) : row(r) {}
   bool operator()(vector<double> a , vector<double>b ) const
   {
    return (a[row]) > (b[row]) ;
   }
};

class mystring: public string{
  public:
  string get_first_line(string in);
  string remove_first_line(string in);
  string remove_first_character(string in);
  mystring():string() {} 
  //string get_first_line (string *in);
  int word_count ( string s );
  string remove_blanks (string in); 
  int count_blanks (string in);
  string remove_first_raute (string in);

};


class ktable:table2d,mystring {

    unsigned int *m,*n;
  string header;
  struct options_type
  {
    int sort;
    int average;
    bool reverse;
    bool line_windows;
    bool header_write;
    bool header_orig;
    double value;
    int precision;
    string outname;
    bool append;
    string appendname;
    char factoropx1;
    char factoropy1;
    char factoropx2;
    char factoropy2;
    
  } options;
  string table_file_read (char *input_filename);
  double s_to_d ( string s, int *lchar, bool *error );
  bool s_to_dvec ( string s, int n, int *last, vector <double> &rvec );
  int s_len_trim (  string s );
  bool ch_eqi ( char c1, char c2 );
  int char_to_digit ( char char_in );
  bool compare_table(const vector<double> &a, const vector <double> &b);
  double op(double in1, double in2, char op_in);
  void table_clear();
  public:
  ktable();
  ktable(const ktable &a);
  ~ktable();
  //vector< vector<double> > table;
  void set_options(char opt, int value);
  void set_options(double value);
  void set_options_string(char opt,string value);
  int get_options(char opt);
  string get_options_string(char opt);
  
  double get_options();
   string table_read(char *input_filename);
  string two_columns(unsigned int j,unsigned int k, ktable *input);
  string table_print(string header,string file);
  string factor(ktable *input,double factor, unsigned int row,char doperator);
   unsigned  int row_count();
  unsigned int column_count();
  string table_sort( unsigned int column);
  string data_arith(char *input_filenamea, unsigned int rowa1, unsigned int rowa2, double factora1, double factora2, char *input_filenameb,unsigned int rowb1, unsigned int rowb2, double factorb1,double factorb2, char doperator);
  //int data_arith(ktable *tablea, ktable *tableb, char doperator);
  void add_row(const vector<double> &row);
  string integral(ktable *input);
  string derivative(ktable *input);
  string dummy(ktable *input);
  string calc_xyf(ktable *input, unsigned int x, int unsigned y, double x_factor, double y_factor,string(ktable::*calc_func)(ktable *input));
  string row_average(ktable *input, unsigned int number_of_rows);
  string row_select(ktable *input, unsigned int column, double value, double derivation);
  string table_get_header(string what);
  string line_end();
  string select_columns(ktable *input,const vector<unsigned int> &columns);
  string table_paste(ktable *input1, ktable *input2);
  string average_two(ktable *input);
  string linearregression(ktable *input, unsigned int x,unsigned int y, double x_factor,double y_factor, unsigned int number_of_rows);
  };

