//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
// Convergence Tests for Monte Carlo results.
//
// Reference
// MCNP(TM) -A General Monte Carlo N-Particle Transport Code
// Version 4B
// Judith F. Briesmeister, Editor
// LA-12625-M, Issued: March 1997, UC 705 and UC 700
// CHAPTER 2. GEOMETRY, DATA, PHYSICS, AND MATHEMATICS
//        VI. ESTIMATION OF THE MONTE CARLO PRECISION
//
// Positives numbers are assumed for inputs
//
// Koi, Tatsumi (SLAC/SCCS)
//

#include "GmConvergenceTester.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

GmConvergenceTester::GmConvergenceTester(G4String name)
 : n(0), sum(0.), mean(0.), var(0.), sd(0.), r(0.), efficiency(0.),
   r2eff(0.), r2int(0.), shift(0.), vov(0.), fom(0.), largest(0.),
   largest_score_happened(0), mean_1(0.), var_1(0.), sd_1(0.), r_1(0.),
   shift_1(0.), vov_1(0.), fom_1(0.), noBinOfHistory(16), slope(0.),
   noBinOfPDF(10), minimizer(0), noPass(0), noTotal(8), theName(name)
{
   nonzero_histories.clear();
   largest_scores.clear();
   largest_scores.push_back( 0.0 );

   history_grid.resize( noBinOfHistory , 0 );
   mean_history.resize( noBinOfHistory , 0.0 );
   var_history.resize( noBinOfHistory , 0.0 );
   sd_history.resize( noBinOfHistory , 0.0 );
   r_history.resize( noBinOfHistory , 0.0 );
   vov_history.resize( noBinOfHistory , 0.0 );
   fom_history.resize( noBinOfHistory , 0.0 );
   shift_history.resize( noBinOfHistory , 0.0 );
   e_history.resize( noBinOfHistory , 0.0 );
   r2eff_history.resize( noBinOfHistory , 0.0 );
   r2int_history.resize( noBinOfHistory , 0.0 );

   timer = new G4Timer();
   timer->Start();
   cpu_time.clear();
   cpu_time.push_back( 0.0 );
}



GmConvergenceTester::~GmConvergenceTester()
{
   delete timer;
}



void GmConvergenceTester::AddScore( G4double x )
{ 

   //G4cout << x << G4endl;

   timer->Stop();
   cpu_time.push_back( timer->GetSystemElapsed() + timer->GetUserElapsed() );

   if ( x == 0.0 ) 
   { 
   }
   else
   {
       nonzero_histories.insert( std::pair< G4int , G4double > ( n , x ) );
       if ( x > largest_scores.back() )  
       { 
//        Following serch  should become faster if begin from bottom.
          std::vector< G4double >::iterator it; 
          for ( it = largest_scores.begin() ; it != largest_scores.end() ; it++ )
          { 
             if ( x > *it ) 
             { 
                largest_scores.insert( it , x );
                break;
             }
          }

          if ( largest_scores.size() > 201 )
          {
             largest_scores.pop_back();
          } 
          //G4cout << largest_scores.size() << " " << largest_scores.front() << " " << largest_scores.back() << G4endl;
       }
       sum += x; 
   }

   n++;
   return; 
}



void GmConvergenceTester::calStat()
{
    

   efficiency = double( nonzero_histories.size() ) / n; 

   mean = sum / n;
   
   G4double sum_x2 = 0.0; 
   var = 0.0;
   shift = 0.0;
   vov = 0.0;

   G4double xi; 
   std::map< G4int , G4double >::iterator it;
   for ( it = nonzero_histories.begin() ; it != nonzero_histories.end() ; it++ )
   {
      xi = it->second;
      sum_x2 += xi * xi;
      var += ( xi - mean ) * ( xi - mean );
      shift += ( xi - mean ) * ( xi - mean ) * ( xi - mean );
      vov += ( xi - mean ) * ( xi - mean ) * ( xi - mean ) * ( xi - mean );
   }

   var += ( n - nonzero_histories.size() ) * mean * mean;
   shift += ( n - nonzero_histories.size() ) * mean * mean * mean * ( -1 );
   vov += ( n - nonzero_histories.size() ) * mean * mean * mean * mean;

   vov = vov / ( var * var ) - 1.0 / n; 

   var = var/(n-1);

   sd = std::sqrt ( var );

   r = sd / mean / std::sqrt ( G4double(n) ); 

   r2eff = ( 1 - efficiency ) / ( efficiency * n );
   r2int = sum_x2 / ( sum * sum ) - 1 / ( efficiency * n );
   
   shift = shift / ( 2 * var * n );
   
   fom =  1 / (r*r) / cpu_time.back(); 

// Find Largest History 
   //G4double largest = 0.0;
   largest = 0.0;
   largest_score_happened = 0;
   G4double spend_time_of_largest = 0.0;
   for ( it = nonzero_histories.begin() ; it != nonzero_histories.end() ; it++ )
   {
      if ( std::abs ( it->second ) > largest )
      {
         largest = it->second;
         largest_score_happened = it->first;
         spend_time_of_largest = cpu_time [ it->first+1 ] - cpu_time [ it->first ];
      }
   }

   mean_1 = 0.0;
   var_1 = 0.0;
   shift_1 = 0.0;
   vov_1 = 0.0;
   sd_1 = 0.0;
   r_1 = 0.0;
   vov_1 = 0.0;

//   G4cout << "The largest history  = " << largest << G4endl;

   mean_1 = ( sum + largest ) / ( n + 1 );

   sum_x2 = 0.0;
   for ( it = nonzero_histories.begin() ; it != nonzero_histories.end() ; it++ )
   {
      xi = it->second;
      sum_x2 += xi * xi;
      var_1 += ( xi - mean_1 ) * ( xi - mean_1 );
      shift_1 += ( xi - mean_1 ) * ( xi - mean_1 ) * ( xi - mean_1 );
      vov_1 += ( xi - mean_1 ) * ( xi - mean_1 ) * ( xi - mean_1 ) * ( xi - mean_1 );
   }
   xi = largest;
   sum_x2 += xi * xi;
   var_1 += ( xi - mean_1 ) * ( xi - mean_1 );
   shift_1 += ( xi - mean_1 ) * ( xi - mean_1 ) * ( xi - mean_1 );
   vov_1 += ( xi - mean_1 ) * ( xi - mean_1 ) * ( xi - mean_1 ) * ( xi - mean_1 );

   var_1 += ( n - nonzero_histories.size() ) * mean_1 * mean_1;
   shift_1 += ( n - nonzero_histories.size() ) * mean_1 * mean_1 * mean_1 * ( -1 );
   vov_1 += ( n - nonzero_histories.size() ) * mean_1 * mean_1 * mean_1 * mean_1;

   vov_1 = vov_1 / ( var_1 * var_1 ) - 1.0 / ( n + 1 ); 

   var_1 = var_1 / n ;

   sd_1 = std::sqrt ( var_1 );

   r_1 = sd_1 / mean_1 / std::sqrt ( G4double(n + 1) ); 

   shift_1 = shift_1 / ( 2 * var_1 * ( n + 1 ) );

   fom_1 = 1 / ( r * r ) / ( cpu_time.back() + spend_time_of_largest );

   if ( nonzero_histories.size() < 500 ) 
   {
      G4cout << "Number of non zero history too small to calcuarte SLOPE" << G4endl;
   }
   else
   {
      G4int i = int ( nonzero_histories.size() );

      // 5% criterion
      G4int j = int ( i * 0.05 );
      while ( int( largest_scores.size() ) > j ) 
      {
         largest_scores.pop_back();
      }
      calc_slope_fit( largest_scores );   
   }

   calc_grid_point_of_history();
   calc_stat_history();
}



void GmConvergenceTester::calc_grid_point_of_history()
{

// histroy_grid [ 0,,,15 ] 
// history_grid [0] 1/16 ,,, history_grid [15] 16/16
// if number of event is x then history_grid [15] become x-1. 
// 16 -> noBinOfHisotry

   G4int i;
   for ( i = 1 ; i <= noBinOfHistory ; i++ )
   { 
      history_grid [ i-1 ] = int ( n / ( double( noBinOfHistory ) ) * i - 0.1 );
      //G4cout << "history_grid " << i-1  << " " << history_grid [ i-1 ] << G4endl;
   }

}



void GmConvergenceTester::calc_stat_history()
{
//   G4cout << "i/16  till_ith  mean  var  sd  r  vov  fom  shift  e  r2eff  r2int" << G4endl;

   G4int i;
   for ( i = 1 ; i <=  noBinOfHistory  ; i++ )
   {

      G4int ith = history_grid [ i-1 ];

      G4int nonzero_till_ith = 0;
      G4double xi;
      G4double mean_till_ith = 0.0;  
      std::map< G4int , G4double >::iterator it;

      for ( it = nonzero_histories.begin() ; it !=nonzero_histories.end() ; it++ )
      {
         if ( it->first <= ith )
         {
            xi = it->second;
            mean_till_ith += xi;
            nonzero_till_ith++; 
         }
      }

      mean_till_ith = mean_till_ith / ( ith+1 ); 
      mean_history [ i-1 ] = mean_till_ith;

      G4double sum_x2_till_ith = 0.0;
      G4double var_till_ith = 0.0;
      G4double vov_till_ith = 0.0;
      G4double shift_till_ith = 0.0;
  
      for ( it = nonzero_histories.begin() ; it !=nonzero_histories.end() ; it++ )
      {
         if ( it->first <= ith )
         {
         xi = it->second;
         sum_x2_till_ith += xi * xi; 
         var_till_ith += ( xi - mean_till_ith ) * ( xi - mean_till_ith );
         shift_till_ith += ( xi - mean_till_ith ) * ( xi - mean_till_ith ) * ( xi - mean_till_ith );
         vov_till_ith += ( xi - mean_till_ith ) * ( xi - mean_till_ith ) * ( xi - mean_till_ith ) * ( xi - mean_till_ith );
         }
      }

      var_till_ith += ( (ith+1) - nonzero_till_ith ) * mean_till_ith * mean_till_ith;

      vov_till_ith += ( (ith+1) - nonzero_till_ith ) * mean_till_ith * mean_till_ith * mean_till_ith * mean_till_ith ;
      vov_till_ith = vov_till_ith / ( var_till_ith * var_till_ith ) - 1.0 / (ith+1); 
      vov_history [ i-1 ] = vov_till_ith;

      var_till_ith = var_till_ith / ( ith+1 - 1 );
      var_history [ i-1 ] = var_till_ith;
      sd_history [ i-1 ] = std::sqrt( var_till_ith );
      r_history [ i-1 ] = std::sqrt( var_till_ith ) / mean_till_ith / std::sqrt ( 1.0*(ith+1) );

      fom_history [ i-1 ] = 1 / ( r_history [ i-1 ] *  r_history [ i-1 ] ) / cpu_time [ ith ];
   
      shift_till_ith += ( (ith+1) - nonzero_till_ith ) * mean_till_ith * mean_till_ith * mean_till_ith * ( -1 );
      shift_till_ith = shift_till_ith / ( 2 * var_till_ith * (ith+1) );
      shift_history [ i-1 ] = shift_till_ith;

      e_history [ i-1 ] = 1.0*nonzero_till_ith / (ith+1);
      r2eff_history [ i-1 ] = ( 1 - e_history [ i-1 ] ) / (  e_history [ i-1 ] * (ith+1) );

      G4double sum_till_ith =  mean_till_ith * (ith+1); 
      r2int_history [ i-1 ] = ( sum_x2_till_ith ) / ( sum_till_ith * sum_till_ith ) - 1 / ( e_history [ i-1 ] * (ith+1) );

   }

}



void GmConvergenceTester::ShowResult()
{
   calStat();

   G4cout << "@@@@ CONVERGENCE TESTER RESULT: " << theName << G4endl;
   G4cout << "EFFICIENCY = " << efficiency << G4endl;
   G4cout << "MEAN = " << mean << G4endl;
   G4cout << "VAR = " << var << G4endl;
   G4cout << "SD = " << sd << G4endl;
   G4cout << "R = "<< r << G4endl;
   G4cout << "SHIFT = "<< shift << G4endl;
   G4cout << "VOV = "<< vov << G4endl;
   G4cout << "FOM = "<< fom << G4endl;

   G4cout << "THE LARGEST SCORE = " << largest << " and it happend at " << largest_score_happened << "th event" << G4endl;
   G4cout << "Affected Mean = " << mean_1 << " and its ratio to orignal is " << mean_1/mean << G4endl;
   G4cout << "Affected VAR = " << var_1 << " and its ratio to orignal is " << var_1/var << G4endl;
   G4cout << "Affected R = " << r_1 << " and its ratio to orignal is " << r_1/r << G4endl;
   G4cout << "Affected SHIFT = " << shift_1 << " and its ratio to orignal is " << shift_1/shift << G4endl;
   G4cout << "Affected FOM = " << fom_1 << " and its ratio to orignal is " << fom_1/fom << G4endl;

   check_stat_history();

// check SLOPE and output result
   if ( slope >= 3 )
   {    
      noPass++;
      G4cout << "SLOPE is large enough" << G4endl; 
   }
   else
   {
      G4cout << "SLOPE is not large enough" << G4endl; 
   }

   G4cout << "This result passes " << noPass << " / "<< noTotal << " Convergence Test." << G4endl; 
   G4cout << G4endl;

}

void GmConvergenceTester::ShowHistory()
{
   G4cout << "@@@@ CONVERGENCE TESTER HISTORY: " << theName << G4endl;
   G4cout << "i/" << noBinOfHistory << " till_ith  mean  var  sd  r  vov  fom  shift  e  r2eff  r2int" << G4endl;
   for ( G4int i = 1 ; i <=  noBinOfHistory  ; i++ )
   {
      G4cout << i << " " 
             << history_grid [ i-1 ] << " "
             << mean_history [ i-1 ] << " "  
             << var_history [ i-1 ] << " "  
             << sd_history [ i-1 ] << " "  
             << r_history [ i-1 ] << " "  
             << vov_history [ i-1 ] << " "  
             << fom_history [ i-1 ] << " " 
             << shift_history [ i-1 ] << " "  
             << e_history [ i-1 ] << " " 
             << r2eff_history [ i-1 ] << " " 
             << r2int_history [ i-1 ] << " " 
             << G4endl;
   }
}

void GmConvergenceTester::check_stat_history()
{

// 1 sigma rejection for null hypothesis 

   std::vector<G4double> first_ally;
   std::vector<G4double> second_ally;

// use 2nd half of hisories
   G4int N = mean_history.size() / 2;
   G4int i;
 
   G4double pearson_r;
   G4double t;

   first_ally.resize( N );
   second_ally.resize( N );

// Mean

   for ( i = 0 ; i < N ; i++ ) 
   {
      first_ally [ i ] = history_grid [ N + i ];
      second_ally [ i ] = mean_history [ N + i ];
   }

   pearson_r = calc_Pearson_r ( N , first_ally , second_ally );
   t = pearson_r * std::sqrt ( ( N - 2 ) / ( 1 - pearson_r * pearson_r ) );

   if ( t < 0.429318 ) // Student t of (Degree of freedom = N-2 )  
   {    
      G4cout << "MEAN distribution is  RANDOM" << G4endl; 
      noPass++;
   }
   else
   {
      G4cout << "MEAN distribution is not RANDOM" << G4endl; 
   }


// R

   for ( i = 0 ; i < N ; i++ ) 
   {
      first_ally [ i ] = 1.0 / std::sqrt ( G4double(history_grid [ N + i ]) );
      second_ally [ i ] = r_history [ N + i ];
   }

   pearson_r = calc_Pearson_r ( N , first_ally , second_ally );
   t = pearson_r * std::sqrt ( ( N - 2 ) / ( 1 - pearson_r * pearson_r ) );
      
   if ( t > 1.090546 )
   {    
      G4cout << "r follows 1/std::sqrt(N)" << G4endl; 
      noPass++;
   }
   else
   {
      G4cout << "r does not follow 1/std::sqrt(N)" << G4endl; 
   }

   if (  is_monotonically_decrease( second_ally ) == true ) 
   {
      G4cout << "r is monotonically decrease " << G4endl;
   }
   else
   {
      G4cout << "r is NOT monotonically decrease " << G4endl;
   }

   G4double Rmin = GmParameterMgr::GetInstance()->GetNumericValue(theName+":Rmin",0.1);
   if ( r_history.back() < Rmin )  
   {
     G4cout << "r is less than " << Rmin << " r = " <<  r_history.back() << G4endl;
      noPass++;
   }
   else
   {
      G4cout << "r is NOT less than " << Rmin << " r = " <<  r_history.back() << G4endl;
   }


// VOV
   for ( i = 0 ; i < N ; i++ ) 
   {
      first_ally [ i ] = 1.0 / history_grid [ N + i ];
      second_ally [ i ] = vov_history [ N + i ];
   }

   pearson_r = calc_Pearson_r ( N , first_ally , second_ally );
   t = pearson_r * std::sqrt ( ( N - 2 ) / ( 1 - pearson_r * pearson_r ) );
      
   if ( t > 1.090546 )
   {    
      G4cout << "VOV follows 1/std::sqrt(N)" << G4endl; 
      noPass++;
   }
   else
   {
      G4cout << "VOV does not follow 1/std::sqrt(N)" << G4endl; 
   }

   if ( is_monotonically_decrease( second_ally ) == true )
   {
      G4cout << "VOV is monotonically decrease " << G4endl;
   }
   else
   {
      G4cout << "VOV is NOT monotonically decrease " << G4endl;
   }

// FOM

   for ( i = 0 ; i < N ; i++ ) 
   {
      first_ally [ i ] = history_grid [ N + i ];
      second_ally [ i ] = fom_history [ N + i ];
   }

   pearson_r = calc_Pearson_r ( N , first_ally , second_ally );
   t = pearson_r * std::sqrt ( ( N - 2 ) / ( 1 - pearson_r * pearson_r ) );

   if ( t < 0.429318 )
   {    
      G4cout << "FOM distribution is RANDOM" << G4endl; 
      noPass++;
   }
   else
   {
      G4cout << "FOM distribution is not RANDOM" << G4endl; 
   }

}



G4double GmConvergenceTester::calc_Pearson_r ( G4int N , std::vector<G4double> first_ally , std::vector<G4double> second_ally )
{
   G4double first_mean = 0.0;  
   G4double second_mean = 0.0;

   G4int i;
   for ( i = 0 ; i < N ; i++ )
   {
      first_mean += first_ally [ i ]; 
      second_mean += second_ally [ i ]; 
   }
   first_mean = first_mean / N;
   second_mean = second_mean / N;
   
   G4double a = 0.0; 
   for ( i = 0 ; i < N ; i++ )
   {
      a += ( first_ally [ i ] - first_mean ) * ( second_ally [ i ] - second_mean );
   }

   G4double b1 = 0.0; 
   G4double b2 = 0.0;
   for ( i = 0 ; i < N ; i++ )
   {
      b1 += ( first_ally [ i ] - first_mean ) * ( first_ally [ i ] - first_mean );
      b2 += ( second_ally [ i ] - second_mean ) * ( second_ally [ i ] - second_mean );
   }
   
   G4double rt = a / std::sqrt ( b1 * b2 );  

   return rt; 
}



G4bool GmConvergenceTester::is_monotonically_decrease ( std::vector<G4double> ally )
{

   std::vector<G4double>::iterator it;
   for ( it = ally.begin() ; it != ally.end() - 1 ; it++ )
   {
      if ( *it < *(it+1) ) return FALSE;
   }

   noPass++;
   return TRUE;
}



//void GmConvergenceTester::calc_slope_fit ( std::vector<G4double> largest_socres )
void GmConvergenceTester::calc_slope_fit ( std::vector<G4double> )
{

   // create PDF bins 
   G4double max = largest_scores.front();
   G4int last = int ( largest_scores.size() );
   G4double min = 0.0;
   if (  largest_scores.back() !=  0 ) 
   {
      min = largest_scores.back();
   }
   else
   {
      min = largest_scores[ last-1 ];
      last = last - 1;
   }
    
   //G4cout << "largest " << max << G4endl;
   //G4cout << "last  " << min << G4endl;

   if ( max*0.99 < min )  
   {
      // upper limit is assumed to have been reached
      slope = 10.0;
      return;
   }

   std::vector < G4double >  pdf_grid;

   pdf_grid.resize( noBinOfPDF+1 );   // no grid  = no bins + 1
   pdf_grid[ 0 ] = max; 
   pdf_grid[ noBinOfPDF ] = min; 
   G4double log10_max = std::log10( max );
   G4double log10_min = std::log10( min );
   G4double log10_delta = log10_max - log10_min;
   for ( G4int i = 1 ; i < noBinOfPDF ; i++ )
   {
      pdf_grid[i] = std::pow ( 10.0 , log10_max - log10_delta/10.0*(i) );    
      //G4cout << "pdf i " << i << " " << pdf_grid[i] << G4endl;
   }
   
   std::vector < G4double >  pdf;
   pdf.resize( noBinOfPDF ); 

   for ( G4int j=0 ; j < last ; j ++ )
   {
      for ( G4int i = 0 ; i < 11 ; i++ )
      {
         if ( largest_scores[j] >= pdf_grid[i+1] )  
         {
            pdf[i] += 1.0 / ( pdf_grid[i] - pdf_grid[i+1] ) / n;
            //G4cout << "pdf " << j << " " << i << " " <<  largest_scores[j]  << " " << G4endl;
            break;
         }
      }
   }

   f_xi.resize( noBinOfPDF );
   f_yi.resize( noBinOfPDF );
   for ( G4int i = 0 ; i < noBinOfPDF ; i++ )
   {
      //G4cout << "pdf i " << i << " " <<  (pdf_grid[i]+pdf_grid[i+1])/2 << " " << pdf[i] << G4endl;
      f_xi[i] = (pdf_grid[i]+pdf_grid[i+1])/2;
      f_yi[i] = pdf[i];
   }

   //                                                  number of variables ( a and k )  
   minimizer = new G4SimplexDownhill<GmConvergenceTester> ( this , 2 ); 
   //G4double minimum =  minimizer->GetMinimum();
   std::vector<G4double> mp = minimizer->GetMinimumPoint();
   G4double k = mp[1];

   //G4cout << "SLOPE " << 1/mp[1]+1 << G4endl;
   //G4cout << "SLOPE  a " << mp[0] << G4endl;
   //G4cout << "SLOPE  k " << mp[1] << G4endl;
   //G4cout << "SLOPE  minimum " << minimizer->GetMinimum() << G4endl;

   slope = 1/mp[1]+1;
   if ( k < 1.0/9 )  // Please look Pareto distribution with "sigma=a" and "k" 
   {
      slope = 10;
   } 
   if ( slope > 10 ) 
   {
      slope = 10; 
   }
}



G4double GmConvergenceTester::slope_fitting_function ( std::vector< G4double > x )
{

   G4double a = x[0];
   G4double k = x[1];

   if ( a <= 0 ) 
   {
      return 3.402823466e+38;  // FLOAT_MAX
   } 
   if ( k == 0 ) 
   {
      return 3.402823466e+38;  // FLOAT_MAX
   }

// f_xi and f_yi is filled at "calc_slope_fit"

   G4double y = 0.0;
   G4int i;
   for ( i = 0 ; i < int ( f_yi.size() ) ; i++ )
   {
      //if ( 1/a * ( 1 + k * f_xi [ i ] / a ) < 0 )
      if ( ( 1 + k * f_xi [ i ] / a ) < 0 )
      {
         y +=3.402823466e+38;  // FLOAT_MAX 
      }
      else 
      {
         y += ( f_yi [ i ] - 1/a*std::pow (  1 + k * f_xi [ i ] / a , - 1/k - 1 ) ) * ( f_yi [ i ] - 1/a*std::pow ( 1 + k * f_xi [ i ] / a , - 1/k - 1 ) );
      }
   }
//   G4cout << "y = " << y << G4endl;

   return y;
}
