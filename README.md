# blockiostat
Simple utility that runs the command passed in the parameters and print IO block statistics upon exit.

Usage example:
  ./blockiostat cp some_path/file_src some_another_path/file_dst

Output example:
  'some_path/file_src' -> 'some_another_path/file_dst'
  Input blocks/sec: 80.542756 
  Output blocks/sec: 101.826539
