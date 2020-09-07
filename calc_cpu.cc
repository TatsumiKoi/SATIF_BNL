#include <iostream>
#include <fstream>

#include <vector>

#include <cmath>
#include <cstdlib>

float cal_ave( std::vector<float> );
float cal_sd( std::vector<float> input , float );

int cal_non_zero( std::vector<float> );
float cal_eff_sd( std::vector<float> , float , int );

int main( int argc , char** argv)
{
   int nmax = 16; //number of job

   if ( argc > 1 ) nmax = atoi ( argv[1] );

   int mmax = 1; //1 data cpu time


   std::vector< std::vector<float>* > v_vp;

   std::fstream data_file ( "data" , std::ios::in ); 

   std::vector< std::string > v_name;
   std::string s;
   data_file >> s;
   v_name.push_back( s );

   int n = 1;
   while ( n <= nmax )
   {
      std::vector<float>* vp =  new std::vector<float>;

      int m = 1;
      while ( m <= mmax )
      {

         if ( !data_file.good() ) 
         {
            nmax = n-1;
           break;
         }
         std::string ss;
         //data_file >> ss;
         //data_file >> ss;
         //data_file >> ss;

         float x=0;
         data_file >> x;
         //std::cout << x << std::endl;
         vp->push_back( x );

         //data_file >> ss;

         //v_name.push_back( s );

       //std::cout << n << " " <<  m << " " << s << " " <<  x << std::endl;
         m++;
      }

      v_vp.push_back( vp );

      n++;
   }

   std::vector<float> v_ave;
   std::vector<float> v_sd;
   std::vector<float> v_efficiency;

   int m = 1;
   while ( m <= mmax ) 

   {
      std::vector< float > vv;
      n = 1;
      while ( n <= nmax )
      {
         vv.push_back( (*(v_vp[(n-1)]))[(m-1)] );
         //std::cout << n << " " << m << " " << (*(v_vp[(n-1)]))[(m-1)] << std::endl;
         n++;
      }
      float ave = cal_ave( vv );
      float sd = cal_sd( vv , ave );
      int no_non_zero = cal_non_zero( vv );
      float efficiency = 1.0*no_non_zero/nmax;
      float eff_ave = ave/efficiency;
      float eff_sd = cal_eff_sd( vv , eff_ave , no_non_zero );
      float eff_sdx = eff_sd/sqrt (no_non_zero);
      float R = eff_sdx / eff_ave;

      v_ave.push_back(ave);
      v_sd.push_back(sd);
      v_efficiency.push_back(efficiency);

      //std::cout << m << " " << ave << " " << sd << " " << no_non_zero << " " << efficiency << " " << eff_ave << " " << eff_sd  << " " << R << std::endl;
//      std::cout << m << " " << ave << " " << R << " " << efficiency << std::endl;
      m++;
   }

//output
   for ( int j = 0 ; j < mmax ; j++ )
   {
      std::cout << v_name[j] << " " << v_ave[j] << " " << 1.0*nmax/(nmax-1)*v_sd[j] << std::endl; 
   }
   //for ( std::vector<float>::iterator it = v_ave.begin(); it != v_ave.end() ; it++ )
   //{
   //   float v1 = *it;
   //   std::cout << v_name[j-1] << " " << v1 << " " << sd << std::endl; 
   //   j++;
   //}
   
   return 0;
} 

float cal_sd( std::vector<float> input , float average )
{
   float result=0;

   for ( std::vector<float>::iterator it = input.begin() ; it != input.end() ; it++ )  
   {
      result += (*it-average)*(*it-average); 
   }

   result = std::sqrt( result/((int)input.size()-1));

   return result;
}



float cal_ave( std::vector<float> input )
{

   float result = 0;

   for ( std::vector<float>::iterator it = input.begin() ; it != input.end() ; it++ )  
   {
      result += *it; 
   }

   result = result/(int)input.size();

   return result;
}


int cal_non_zero( std::vector<float> input )
{

   int n = 0;
   for ( std::vector<float>::iterator it = input.begin() ; it != input.end() ; it++ )  
   {
      if ( *it != 0 ) n++;
   }

   return n;
}

float cal_eff_sd( std::vector<float> input , float average , int n )
{
   float result=0;

   for ( std::vector<float>::iterator it = input.begin() ; it != input.end() ; it++ )  
   {
      if ( *it != 0 )
      {
         result += (*it-average)*(*it-average); 
      }
   }

   result = std::sqrt( result/(n-1));

   return result;
}
