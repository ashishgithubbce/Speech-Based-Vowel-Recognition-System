                                                                         Assignment_03:  Vowel Recognition
Methods Name and Their Functionality:
->Methods used for Training Purpose
 1. Dc_shift(): find average Dc shift in Silence signal 
2.normalization(): to Normalize the Signal on scale of  10,000  Amplitude 
3.find_Ri(): Find Ri values of each frame of Signal
4.find_Ai(): find Ai value of each Frame of Signal,Which is taking input Ri values
5.find_Ci(): find Ci Value of each Frame,which is taking input Ai Values(by Appling Raised Sine Window)
6. input_steady_frame(): find  input  Steady Frame, which is taking input as amplitude count Value from where steady frame start
7.find_steady_frame(): find Steady frame, which is taking input as Normalize signal
8.find_Avg_Ci(): find Average Ci values with respect to the frames
->Methods Used For Testing Purpose
9.cal_tokhura_distence():  calculate Tokura Distence Between reference vowel and test vowel based on Ci values
10. vowel_recognise(): Method to recognise vowel by passing Argument as a test file and Tokhura Weights

->File Naming convention used to Generate Reference_files and Test_Files:
1.Dc shift file: 204101015_vowel_DcShift_file_num(1-20)
2.Normalized File: 204101015_vowel_normalize_file_num
3.Reference File for Ci: 204101015_Ci_vowel_reference_+Ai //+Ai means Ci  values Calculated Without Nagation of Ai Values
4.Test File: 204101015_Ci_vowel_test_+Ai_file_num(11-20)

->Steps to Execute Code for Testing  vowels:
1.Set the file path of reference file of each vowel manually in the vowel_recognise() method.
2. To test each vowel(10 test file of each Vowel),we have call vowel_recognise() method which is Declare in Main() method, and we are using nested loop  for testing the vowels, 
the outer loop tells the particular  vowel that we want to test and inner loop execute 10 time for each  vowel as a test File.
3.there are Total 50 test file(10 test file for each Vowel) we have test, out of 50 test file how many test vowel is Correctly Recognized is Display on Console to find Accuracy.
