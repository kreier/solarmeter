# Data base for measured values

The recorded data since June 2020 is stored both in a database and a google sheet. The parameters are the same:

- Each month gets a table/sheet, named YYYYMM like 202006
- The first column is the timestamp in Y2000 format YYYYMMDDHHMMSS
- Values are measured every minute and the average is stored in the database every 5 minutes
- The values are assigned to one of the 288 time slots per day
- For months with 31 days this results in 8928 rows
