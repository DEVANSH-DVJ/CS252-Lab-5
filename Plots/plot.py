import matplotlib.pyplot as plt

per = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
q1d = [5.45414, 10.8344, 16.3675, 19.6522, 19.8636, 19.889, 19.833, 20.0087, 20.0495, 20.1284]
q1e = [5.45415, 10.9108, 16.3675, 21.4805, 22.0457, 22.0967, 22.145, 22.252, 22.2444, 22.2138]
q2d = [5.45414, 10.9108, 16.3675, 21.8242, 27.1943, 28.2281, 28.0193, 28.1874, 28.2078, 28.111]
q2e = [5.45414, 10.9108, 16.3675, 21.8242, 25.4145, 25.3305, 25.2744, 25.4603, 25.3483, 25.5163]
q3 = [5.44651, 10.9006, 16.3548, 21.8064, 27.258, 31.2837, 33.145, 34.8485, 36.4195, 36.3559]
q4 = [5.45414, 10.9108, 16.3675, 17.9492, 17.5159, 18.1397, 19.7821, 21.9974, 22.5041, 22.5474]

plt.plot(per, q1d, color='blue', label='RTS/CTS disabled')
plt.plot(per, q1e, color='red', label='RTS/CTS enabled')
plt.ylabel('Total throughput (in Mbps)')
plt.xlabel('% channel data rate')
plt.title('Question 1 - Hidden Terminal')
plt.legend()
plt.grid()
plt.show()

plt.plot(per, q2d, color='blue', label='RTS/CTS disabled')
plt.plot(per, q2e, color='red', label='RTS/CTS enabled')
plt.ylabel('Total throughput (in Mbps)')
plt.xlabel('% channel data rate')
plt.title('Question 2 - No Hidden Terminal')
plt.legend()
plt.grid()
plt.show()

plt.plot(per, q3, color='blue', label='RTS/CTS enabled')
plt.ylabel('Total throughput (in Mbps)')
plt.xlabel('% channel data rate')
plt.title('Question 3 - Wi-fi Scenario 1')
plt.legend()
plt.grid()
plt.show()

plt.plot(per, q4, color='blue', label='RTS/CTS enabled')
plt.ylabel('Total throughput (in Mbps)')
plt.xlabel('% channel data rate')
plt.title('Question 4 - Wi-fi Scenario 2')
plt.legend()
plt.grid()
plt.show()
