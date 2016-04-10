# -*- coding: utf-8 -*-
"""
python code using pyspark to investigate top blocks with crime events in chicago, 
correlations between beats, and comparison in narcotics related arrests between 
mayors daley and emanuel
@author: msnyder
"""

from pyspark.sql import SQLContext,Row
from pyspark import SparkContext
from pyspark.mllib.stat import Statistics
from pyspark.mllib.linalg import Vectors
import numpy as np
import datetime

sc = SparkContext()
sqlContext = SQLContext(sc)

# read in csv
crime_data_lines = sc.textFile('Crimes_-_2001_to_present.csv')

# extract relevant columns from the data 
crime_data_parts = crime_data_lines.map(lambda line: line.split(","))
crimes = crime_data_parts.map(lambda p: Row(TEMPORAL=p[2],BLOCK=p[3],BEAT=p[10]))

###############################################################################
# part(a) finding top ten blocks for crime

# remove header
header = crimes.first()
crimes = crimes.filter(lambda x: x != header)

# extract year from temporal field
def filter_years(temporal):
    year = int(temporal[6:10])
    if year >= 2012 and year <= 2015:
        return True
    else:
        return False

# strip off end of block column and return only the block id
def get_block(block):
    return block[:5]
    
#5,801,845 records before
crimes = crimes.filter(lambda x:filter_years(x[2]))
#1,005,615 records after

blocks = crimes.map(lambda x:get_block(x[1]))
blocks_kv = blocks.map(lambda block: (block, 1))
block_counts = blocks_kv.reduceByKey(lambda x, y: x + y)
block_counts_switched = block_counts.map(lambda x: (x[1], x[0]))
all_blocks = block_counts_switched.sortByKey(ascending=False)
top_blocks = all_blocks.take(10)
                            
#print (top_blocks)
# [(30367, u'0000X'), (22413, u'001XX'), (18603, u'002XX'), (18090, u'008XX'),
# (17164, u'003XX'), (16388, u'016XX'), (16297, u'012XX'), (16172, u'013XX'), 
# (16115, u'007XX'), (15991, u'011XX')]

###############################################################################
# part(b) finding beats in Chicago with the highest correlation between crime events, by year

crimes = crime_data_parts.map(lambda p: Row(TEMPORAL=p[2],BEAT=p[10]))
header = crimes.first()
crimes = crimes.filter(lambda x: x != header)

# extract year from temporal field
def extract_year(temporal):
    year = int(temporal[6:10])
    return year
        
crimes = crimes.map(lambda x:(x[0],extract_year(x[1])))
crimes_by_beat_year = crimes.map(lambda x: ((x[0],x[1]),1))
crime_counts_by_beat_year = crimes_by_beat_year.reduceByKey(lambda x,y: x + y)
# sample data at this point, crime counts by beat and year
#[((u'0113', 2008), 1199), ((u'1814', 2005), 880), ((u'1022', 2001), 1675),

beats_by_year = crime_counts_by_beat_year.map(lambda x: (x[0][0],(x[0][1],x[1]))).groupByKey() 
beats_by_year_grouped = beats_by_year.mapValues(lambda x: sorted(list(x), key=lambda y: y[0]))
filtered = beats_by_year_grouped.filter(lambda x: len(x[1]) == 15)
# filter out beats that don't have data for all 15 years

filtered_sorted = filtered.map(lambda x: x).sortByKey()
# sort by beat so that adjacent beats are next to each other
#print (filtered_sorted.take(15))
# sample output for one beat 
#[(u'0111', [(2001, 1584), (2002, 1327), (2003, 1272), (2004, 1164), (2005, 1089), 
#(2006, 1090), (2007, 1313), (2008, 1167), (2009, 1207), (2010, 1132), (2011, 1028), 
#(2012, 1430), (2013, 1625), (2014, 1616), (2015, 526)])

# noticed that one tuple has a key of 'true' so filter that out
filtered_more = filtered_sorted.filter(lambda x: x[0] != 'true')

def collapse_years(x):
    # for reshaping the rdd so that we can pass it to correlation function
    year_beat_crimes = tuple([(i[0],i[1]) for i in x[1]])
    return year_beat_crimes

years_crimes = filtered_more.flatMap(lambda x: (collapse_years(x)))
#print (years_crimes.collect())
# [(2001, 1584), (2002, 1327), (2003, 1272), (2004, 1164), (2005, 1089), 
# (2006, 1090), (2007, 1313), (2008, 1167), (2009, 1207), (2010, 1132), 
# (2011, 1028), (2012, 1430), (2013, 1625), (2014, 1616), (2015, 526), 
# (2001, 1720), (2002, 1679), (2003, 1691), (2004, 1412), (2005, 1172), 
# (2006, 1169), (2007, 1260), (2008, 1541), (2009, 1583), (2010, 1432), 
# (2011, 1327), (2012, 1124), (2013, 942), (2014, 891), (2015, 339), 

years_crimes_grouped = years_crimes.groupByKey()
for_vectors = years_crimes_grouped.mapValues(lambda x: sorted(list(x), key=lambda y: y))

crime_vectors = for_vectors.map(lambda x: Vectors.dense([x[1]])) 
beat_correlations = Statistics.corr(crime_vectors,method="pearson")
# print (np.shape(beat_correlations))
# produces a 254x254 array of correlations

def get_max_corr(x):
    max_corr = 0
    for (i,j), values in np.ndenumerate(x):
        if x[i][j] > max_corr and x[i][j] < 1 and abs(i-j) == 1:
            max_corr = x[i][j]
    return max_corr

max_correlation = get_max_corr(beat_correlations)
max_indices = np.where(beat_correlations==max_correlation)

#print (max_correlation)
#print (max_indices)
# max correlation in the array is 0.999985385563, occurs at row 93 column 94

beats_list = filtered_more.collect()

#print (beats_list[93][0])
#print (beats_list[94][0])

adjacent_beats = [beats_list[93][0],beats_list[94][0]]

# max correlation occurs between beats 0833 and 0834, which are adjacent beats
# near Midway airport in Chicago according to http://gis.chicagopolice.org/pdfs/district_beat.pdf

###############################################################################
# part(c) investigating crime events between mayors daly and emanuel
# mayor daly left office on may 16, 2011
# going to investigate whether the percentage of total arrests that are classified 
# as 'narcotics' has increased or decreased since mayor rahm emanuel took office

# extract relevant columns from the data 
for_extraction = crime_data_lines.map(lambda line: line.split(","))
date_descriptions_arrest = for_extraction.map(lambda p: Row(TEMPORAL=p[2],ARREST_TYPE=p[5],ARRESTED=p[8]))
header_c = date_descriptions_arrest.first()
descriptions = date_descriptions_arrest.filter(lambda x: x != header_c)
# [Row(ARRESTED=u'true', ARREST_TYPE=u'WEAPONS VIOLATION', TEMPORAL=u'05/19/2015 11:57:00 PM'),

def convert_time(x):
    converted_time = datetime.datetime.strptime(x, '%m/%d/%Y %I:%M:%S %p')
    return converted_time
    
def is_daley(x):
    daley = True
    if convert_time(x) >= datetime.datetime.strptime('05/16/2011', '%m/%d/%Y'):
        return False
    return daley

def get_year(x):
    return convert_time(x).year
    
mayors_converted = descriptions.map(lambda x: (x[0],x[1],get_year(x[2]),is_daley(x[2])))
mayors_converted_arrests = mayors_converted.filter(lambda x: x[0]=='true') 
#print (mayors_converted.take(10))
#[(u'true', u'WEAPONS VIOLATION', 2015, False), (u'true', u'INTERFERENCE WITH PUBLIC OFFICER', 2015, False),

# filter anything that isn't an arrest due to narcotics
narcotics_arrests = mayors_converted.filter(lambda x: x[0]=='true')
narcotics_arrests = narcotics_arrests.filter(lambda x: x[1]=='NARCOTICS')
#print (total_crimes.count())
# 1,620,633 total arrests in past 15 years
#print (narcotics_arrests.count())
# 643,046 narcotics arrests in the past 15 years
# [(u'true', u'NARCOTICS', 2015, False), (u'true', u'NARCOTICS', 2015, False),

total_crimes_kv = mayors_converted_arrests.map(lambda x: (str(x[2])+'_'+str(x[3]),1))
narcotics_crimes_kv = narcotics_arrests.map(lambda x: (str(x[2])+'_'+str(x[3]),1))

total_counts = total_crimes_kv.reduceByKey(lambda x, y: x + y)
total_crime_counts = total_counts.sortByKey(ascending=True)

narcotics_counts = narcotics_crimes_kv.reduceByKey(lambda x, y: x + y)
narcotics_crime_counts = narcotics_counts.sortByKey(ascending=True)

#print (total_crime_counts.collect())
#[('2001_True', 137835), ('2002_True', 137074), ('2003_True', 136996), 
#('2004_True', 140005), ('2005_True', 136159), ('2006_True', 131135), 
#('2007_True', 127741), ('2008_True', 106084), ('2009_True', 106938), 
#('2010_True', 96709), ('2011_False', 60012), ('2011_True', 32965), 
#('2012_False', 87595), ('2013_False', 83387), ('2014_False', 75525), 
#('2015_False', 24473)]
#print (narcotics_crime_counts.collect())
#('2001_True', 49753), ('2002_True', 51022), ('2003_True', 53374), 
#('2004_True', 56110), ('2005_True', 55253), ('2006_True', 54435), 
#('2007_True', 52399), ('2008_True', 44708), ('2009_True', 42416), 
#('2010_True', 42333), ('2011_False', 23414), ('2011_True', 14371), 
#('2012_False', 34348), ('2013_False', 32935), ('2014_False', 27491),
# ('2015_False', 8684)]

zipped = narcotics_crime_counts.zip(total_crime_counts)
narcotics_arrests_percentages = zipped.map(lambda x: (x[0][0],float(x[0][1])/float(x[1][1])))
#print (narcotics_arrests_percentages.collect())
#[('2001_True', 0.36096056879602423), ('2002_True', 0.37222230328143924), 
# ('2003_True', 0.38960261613477765), ('2004_True', 0.40077140102139208), 
# ('2005_True', 0.40579763364889576), ('2006_True', 0.41510656956571473), 
# ('2007_True', 0.41019719588855574), ('2008_True', 0.42143961389087892), 
# ('2009_True', 0.39664104434345132), ('2010_True', 0.43773588807660091), 
# ('2011_False', 0.39015530227287876), ('2011_True', 0.43594721674503262),
# ('2012_False', 0.3921228380615332), ('2013_False', 0.39496564212647056), 
# ('2014_False', 0.36399867593512081), ('2015_False', 0.35484002778572304)]

print (top_blocks)
print (adjacent_beats)
print (narcotics_arrests_percentages.collect())