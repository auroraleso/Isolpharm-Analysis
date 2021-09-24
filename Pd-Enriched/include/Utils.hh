#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <sstream>
#include <getopt.h>
#include <iostream>

#include "colormod.h"
#include "CompassEvent.hh"
#include "RunInformation.hh"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void print_usage(const char *name)
{
    std::cout << "Usage: " << name << " [options] <argument>\n" << std::endl;
    std::cout << "CoMPASS Data Reader\n" << std::endl;
    std::cout << std::endl;
    std::cout << "Optional arguments:\n" << std::endl;
    std::cout << "\t-H: Display this message.\n" << std::endl;
    std::cout << "\t-i: Path to the CoMPASS run.info file.\n" << std::endl;
    std::cout << "\t-s: Start date in the format : Year/Month/Day hours:minutes:seconds.\n" << std::endl;
    std::cout << "\t-e: End date in the format : Year/Month/Day hours:minutes:seconds.\n" << std::endl;
    std::cout << "\t-f: ROOT measurements file.\n" << std::endl;
    std::cout << "\t-N: Acquisition Length, in seconds.\n" << std::endl;
    std::cout << "\t-S: Scan the whole file with a given time window in seconds.\n" << std::endl;

    return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string from_unix_to_date(time_t t)
{
    struct tm *tm = localtime(&t);
    char date[20];
    strftime(date, sizeof(date), "%Y_%m_%d_%H_%M_%S", tm);

    return "h_"+std::string(date);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void printProgBar( float percent, float a, float b, float c)
{
  std::string bar;

  for(int i = 0; i < 50; i++){
    if( i < (int(percent)/2)){
      bar.replace(i,1,"=");
    }else if( i == int(percent/2)){
      bar.replace(i,1,">");
    }else{
      bar.replace(i,1," ");
    }
  }

  Color::Modifier blue1(Color::FG_BLUE);
  Color::Modifier def(Color::FG_DEFAULT);
  Color::Modifier blue2(Color::FG_BLUE);

  std::cout<<"\r [" << blue1 << bar << def <<"]";
  std::cout.width( 3 );
  std::cout<< std::fixed;
  std::cout<< std::setprecision(2);

  if ( a == -100 && b == -100 && c == -100 ) {
    std::cout<< blue2 << percent << def << "%"<< std::flush;
  }
  if ( a != -100 && b == -100 && c == -100 ) {
    std::cout<< blue2 << percent << def << "% -- par#1 = " << a << std::flush;
  }
  if ( a != -100 && b != -100 && c == -100 ) {
    std::cout<< blue2 << percent << def << "% -- par K " << a <<", par L - "<<b<< std::flush;
  }
  if ( a != -100 && b != -100 && c != -100 ) {
    std::cout<< blue2 << percent << def << "% -- unix_timestamp_event " << a << " time start user " << b << " time start DAQ " << c << std::flush;
  }

  return ;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

time_t to_unix_timestamp(std::string time_string)
{
    time_t ts;

    std::tm t = {};
    std::istringstream iss(time_string);
    std::put_time(&t, "%c");
    iss >> std::get_time(&t, "%Y/%m/%d %H:%M:%S");

    ts = mktime(&t);

    return ts;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

RunInformation GetRunInformation(std::string filename)
{
    RunInformation ri;

    std::ifstream file_info(filename.c_str());

    std::string line, buffer;

    while(std::getline(file_info, line))
    {
        std::istringstream iss(line);

        if(line.rfind("id",0) == 0)
        {
            ri.name = line.substr(line.find("=")+1,line.size());
        }

        if(line.rfind("time.start",0) == 0)
        {
            ri.time_start_UTC = line.substr(line.find("=")+1,line.size());
            line = line.substr(line.find("=")+1, line.find("+")-14);
            ri.start = to_unix_timestamp(line);
        }

        if(line.rfind("time.stop",0) == 0)
        {
            ri.time_stop_UTC = line.substr(line.find("=")+1,line.size());
            line    = line.substr(line.find("=")+1, line.find("+")-14);
            ri.stop = to_unix_timestamp(line);
        }

    }

    return ri;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
