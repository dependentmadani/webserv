#!/usr/bin/env perl

use strict;
use warnings;
use CGI;

print "OK, starting time consuming process ... \n";
# Tell Perl not to buffer our output

my $counter = 10;

while($counter > 0){

   print("$counter\n");

   # count down
   $counter--;

   # pause program for 1 second
   sleep(1);

   if($counter == 0){
	print("Happy New Year!\n");
   }
}
print "All Done!\n";