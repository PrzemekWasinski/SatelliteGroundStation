# Script 1:
- Contains API functions
- Contains file transfer functions

# Script 2:
- Contains CSV Writing/Reading functions

# Script 3:
- Contain SatDump controlling functions

# Main Script:

## Every 30 min:
- Calls API function every
- Calls CSV write function to write results to csv

## Every 1 min:
- Reads csv to check if 'currentTime' > 'passStart' -> Starts recording if true
- Checks if 'currentTime' > 'passEnd' -> Stops recording if true
- Decodes the recording into images
- Deletes recording file
- Transfers images to storage server
- Deletes Images
- Deletes all rows where 'currentTime' > 'passEnd'
