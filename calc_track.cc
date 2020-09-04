#include <iostream>
#include <fstream>

#include <vector>

#include <cmath>
#include <cstdlib>

double cal_ave( std::vector<double> );
double cal_sd( std::vector<double> input , double );

int cal_non_zero( std::vector<double> );
double cal_eff_sd( std::vector<double> , double , int );

int main( int argc , char** argv)
{
   int nmax = 16; //number of job

   if ( argc > 1 ) nmax = atoi ( argv[1] );

   int mmax = 16; //number of layers per jobs
   int lmax = 12; //number of data per layers

/*   if ( argc > 2 )
   {
      if ( 61 == atoi ( argv[2] ) ) mmax = 21*6+1;
   }
*/

   std::vector< std::vector<double>* > v_vp;

   std::fstream data_file ( "data" , std::ios::in ); 

   std::vector< std::string > v_name;

   int n = 1;
   while ( n <= nmax ) // loop for jobs
   {
      std::vector<double>* vp =  new std::vector<double>;

      int m = 1;
      while ( m <= mmax ) // loop for data 
      {

         if ( m == 1 ) {
            //Header 
            std::string s;
            for ( int i = 0 ; i < 26 ; i++ ) {
               data_file >> s;
               if ( n == 1 && i/2*2 == i ) { 
                  v_name.push_back( s );
               }
            }
            if ( !data_file.good() ) {
               nmax = n-1;
              break;
            }
         } else {
            //Data

            std::string sdummy;
            data_file >> sdummy;
            data_file >> sdummy;
            for ( int i = 0 ; i < lmax ; i++ ) {
            double x=0;
            data_file >> x;
            vp->push_back( x );
            data_file >> sdummy;
         }

      //if ( m <= 56 ) v_name.push_back( s );

         }
      //std::cout << n << " " <<  m << " " << s << " " <<  x << " " << y << std::endl;
         m++;

      }

      v_vp.push_back( vp );

      n++;
   }

   std::vector<double> v_ave;
   std::vector<double> v_sd;
   std::vector<double> v_efficiency;

   int m = 1;
   while ( m <= (mmax-1)*lmax ) // 
   {
      std::vector< double > vv;
      n = 1;
      while ( n <= nmax ) {
         vv.push_back( (*(v_vp[(n-1)]))[(m-1)] );
         //std::cout << n << " " << m << " " << (*(v_vp[(n-1)]))[(m-1)] << std::endl;
         n++;
      }
      double ave = cal_ave( vv );
      double sd = cal_sd( vv , ave );
      int no_non_zero = cal_non_zero( vv );
      double efficiency = 1.0*no_non_zero/nmax;
      double eff_ave = ave/efficiency;
      double eff_sd = cal_eff_sd( vv , eff_ave , no_non_zero );
      double eff_sdx = eff_sd/sqrt (no_non_zero);
      double R = eff_sdx / eff_ave;

      v_ave.push_back(ave);
      v_sd.push_back(sd);
      v_efficiency.push_back(efficiency);

      //std::cout << m << " " << ave << " " << sd << " " << no_non_zero << " " << efficiency << " " << eff_ave << " " << eff_sd  << " " << R << std::endl;
//      std::cout << m << " " << ave << " " << R << " " << efficiency << std::endl;
      m++;
   }

//output
// Header
   for ( int il = 0 ; il <= lmax ; il++ ) {
      std::cout << v_name[il] << " "; 
   }
   std::cout << std::endl;

// results
   std::vector<double> v_sum(lmax,0);
   for ( int im = 0 ; im < mmax-1 ; im++ ) {
      //std::cout << im; 
      for ( int il = 0 ; il < lmax ; il++ ) {
         int i = im*lmax+il;
         //std::cout << " " << v_ave[i] << " " << 1.0*nmax/(nmax+1)*v_sd[i];
         v_sum[il]+=v_ave[i];
      }
      //std::cout << std::endl; 
   }

   std::cout << "ALL"; 
   for ( int il = 0 ; il < lmax ; il++ ) {
      std::cout << " " << v_sum[il];
   }
   std::cout << std::endl; 
   
   return 0;
} 



double cal_sd( std::vector<double> input , double average )
{
   double result=0;

   for ( std::vector<double>::iterator it = input.begin() ; it != input.end() ; it++ )  
   {
      result += (*it-average)*(*it-average); 
   }

   result = std::sqrt( result/((int)input.size()-1));

   return result;
}



double cal_ave( std::vector<double> input )
{

   double result = 0;

   for ( std::vector<double>::iterator it = input.begin() ; it != input.end() ; it++ )  
   {
      result += *it; 
   }

   result = result/(int)input.size();

   return result;
}


int cal_non_zero( std::vector<double> input )
{

   int n = 0;
   for ( std::vector<double>::iterator it = input.begin() ; it != input.end() ; it++ )  
   {
      if ( *it != 0 ) n++;
   }

   return n;
}

double cal_eff_sd( std::vector<double> input , double average , int n )
{
   double result=0;

   for ( std::vector<double>::iterator it = input.begin() ; it != input.end() ; it++ )  
   {
      if ( *it != 0 )
      {
         result += (*it-average)*(*it-average); 
      }
   }

   result = std::sqrt( result/(n-1));

   return result;
}
