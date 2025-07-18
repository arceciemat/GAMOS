import sys
import numpy as np
import os

###-----------------------------------------------------
def groupAndAverage(X, Y, interval_size,x_min=sys.float_info.min,x_max=sys.float_info.max) :
    # Convert lists to numpy arrays
    X = np.array(X)
    Y = np.array(Y)

    # Find the maximum and minimum values of X, if not set
    if x_min == sys.float_info.min :
        x_min = np.min(X)
    if x_max == sys.float_info.max :
        x_max = np.max(X)

    # Create bins
    bins = np.arange(x_min, x_max + interval_size, interval_size)

    # Digitize X into bins
    digitized = np.digitize(X, bins)

    # Initialize arrays to store averages and counts
    averages = np.zeros(len(bins) - 1)
    counts = np.zeros(len(bins) - 1)

    # Calculate average Y for each bin
    for i in range(1, len(bins)):
        indices = np.where(digitized == i)
        if len(indices[0]) > 0:
            averages[i - 1] = np.mean(Y[indices])
            counts[i - 1] = len(indices[0])

    return bins[:-1], averages, counts

# Example usage
#X = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
#Y = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
#interval_size = 2

#bins, averages, counts = groupAndAverage(X, Y, interval_size)
#for i in range(len(bins)):
#    print(f"Interval {bins[i]} - {bins[i] + interval_size}: Average Y = {averages[i]}, Count = {counts[i]}")

##------------------------------------------------------------
def interpolateLineSegments(x, x_values, y_values) :
    #print(len(x_values),"InterpolateLineSegments",x,x_values) #GDEB
    if len(x_values) == 0 :
        print("!!! FATAL ERROR interpolateLineSegments x_values is empty")
        sys.exit()
    if len(y_values) == 0 :
        print("!!! FATAL ERROR interpolateLineSegments y_values is empty")
        sys.exit()
    if x <= x_values[0]:
        return y_values[0]
    elif x >= x_values[-1]:
        return y_values[-1]
    else:
        idx = np.searchsorted(x_values, x)
        x0, x1 = x_values[idx - 1], x_values[idx]
        y0, y1 = y_values[idx - 1], y_values[idx]
        return y0 + (y1 - y0) / (x1 - x0) * (x - x0)

##------------------------------------------------------------
import fnmatch
def filesWithString(directory, search_string):
    matching_files = []
    # List all files in the directory (not recursively)
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        if os.path.isfile(file_path) and search_string in filename:
            #print(len(matching_files),"filesWithString ADD",file_path) #GDEB
            matching_files.append(file_path)
    return matching_files

def filesWithStringRecursive(directory, search_string):
    matching_files = []
    for root, dirs, files in os.walk(directory):
        for filename in fnmatch.filter(files, f'*{search_string}*'):
            matching_files.append(os.path.join(root, filename))
            print(" filesWithString(",directory, search_string,filename)
    return matching_files

def filesWithStrings(directory, search_string1, search_string2):
    matching_files = []
    try:
        # List all files in the directory (not recursively)
        for filename in os.listdir(directory):
            file_path = os.path.join(directory, filename)
            if os.path.isfile(file_path) and search_string1 in filename and search_string2 in filename:
                matching_files.append(file_path)
    except FileNotFoundError:
        print(f"The directory {directory} does not exist.")
    except PermissionError:
        print(f"Permission denied to access the directory {directory}.")
    return matching_files

def filesWithStringsRecursive(directory, search_string1, search_string2):
    matching_files = []
    for root, dirs, files in os.walk(directory):
        for filename in fnmatch.filter(files, f'*{search_string1}*{search_string2}'):
            matching_files.append(os.path.join(root, filename))
    return matching_files
#def FilesWithString(directory, search_string):
 #   return filesWithString(directory, search_string)

import glob
def filesWithString2(directory, search_string):
    pattern = os.path.join(directory, f'*{search_string}*')
    return glob.glob(pattern)

#def FilesWithString2(directory, search_string):
 #   return filesWithString2(directory, search_string)

def firstLevelSubDirsWithString(directory, search_string):
    try:
        # List all entries in the directory
        entries = os.listdir(directory)
        # Filter out only directories that contain the search string
        subdirectories = [
            entry for entry in entries
            if os.path.isdir(os.path.join(directory, entry)) and search_string in entry
        ]
        return subdirectories
    except Exception as e:
        print(f"An error occurred: {e}")
        return []
    
##------------------------------------------------------------
def gauss(x, A, x0, sigma):
    return A * np.exp(-(x - x0) ** 2 / (2 * sigma ** 2))

