import matplotlib.pyplot as plt

per = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
q1d = [5.4541, 10.8344, 16.3675, 19.6522, 19.8636, 19.8890, 19.8330, 20.0087, 20.0495, 20.1284]
q1e = [5.4541, 10.9108, 16.3675, 21.4805, 22.0457, 22.0967, 22.1450, 22.2520, 22.2444, 22.2138]

q2d = [5.4541, 10.9108, 16.3675, 21.8242, 27.1943, 28.2281, 28.0193, 28.1874, 28.2078, 28.1110]
q2e = [5.4541, 10.9108, 16.3675, 21.8242, 25.4145, 25.3305, 25.2744, 25.4603, 25.3483, 25.5163]

q3a = [1.8180, 3.6386, 5.4592, 7.2798, 9.1004, 10.9185, 12.7391, 14.5597, 16.3803, 18.2009]
q3b = [1.8155, 3.6335, 5.4516, 7.2671, 9.0852, 9.5486, 7.7050, 5.6044, 4.0282, 0.0076]
q3c = [1.8130, 3.6285, 5.4440, 7.2595, 9.0724, 10.8879, 12.7034, 14.5189, 16.3344, 18.1474]
q3t = [5.4465, 10.9006, 16.3548, 21.8064, 27.2580, 31.3550, 33.1475, 34.6830, 36.7429, 36.3559]

q4a = [2.7296, 5.4592, 8.1888, 7.0430, 3.8856, 1.7824, 0.6977, 0.1884, 0.0331, 0.0255]
q4c = [2.7245, 5.4516, 8.1787, 10.9032, 13.6303, 16.3573, 19.0844, 21.8090, 22.4710, 22.5219]
q4t = [5.4541, 10.9108, 16.3675, 17.9462, 17.5159, 18.1397, 19.7821, 21.9974, 22.5041, 22.5474]

plt.figure(figsize=(20,10))
plt.plot(per, q1d, color='blue', label='RTS/CTS disabled')
plt.plot(per, q1e, color='red', label='RTS/CTS enabled')
plt.ylabel('Total throughput (in Mbps)')
plt.xlabel('% channel data rate')
plt.title('Question 1 - Hidden Terminal')
plt.legend()
plt.grid()
plt.show()

plt.figure(figsize=(20,10))
plt.plot(per, q2d, color='blue', label='RTS/CTS disabled')
plt.plot(per, q2e, color='red', label='RTS/CTS enabled')
plt.ylabel('Total throughput (in Mbps)')
plt.xlabel('% channel data rate')
plt.title('Question 2 - No Hidden Terminal')
plt.legend()
plt.grid()
plt.show()

plt.figure(figsize=(20,10))
plt.plot(per, q3t, color='blue', label='Total throughput')
plt.plot(per, q3a, color='red', label='Flow 1')
plt.plot(per, q3b, color='green', label='Flow 2')
plt.plot(per, q3c, color='orange', label='Flow 3')
plt.ylabel('Total throughput (in Mbps)')
plt.xlabel('% channel data rate')
plt.title('Question 3 - Wi-fi Scenario 1')
plt.legend()
plt.grid()
plt.show()

plt.figure(figsize=(20,10))
plt.plot(per, q4t, color='blue', label='Total throughput')
plt.plot(per, q4a, color='red', label='Flow 1')
plt.plot(per, q4c, color='orange', label='Flow 2')
plt.ylabel('Total throughput (in Mbps)')
plt.xlabel('% channel data rate')
plt.title('Question 4 - Wi-fi Scenario 2')
plt.legend()
plt.grid()
plt.show()
