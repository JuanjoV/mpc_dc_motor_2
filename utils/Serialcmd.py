
from serial import Serial
import time

class zynqTest():
	def __init__(self, port='COM8', baudrate=115200 ,tests=1):
		self.serial = Serial(port=port, baudrate=baudrate)
		self.u = []
		self.x = []
		self.refu = []
		self.refx = []
		self.tests = tests
		self.initialConditions = []
		self.timeEl = []

	def sendVector(self, vector):
		for element in vector:
			self.serial.write( (str(element)+"\n").encode('ascii'))
			time.sleep(0.01)
	
	def receiveResult(self):
		line = self.serial.readline().decode('ascii')
		while (line == None or line == "\n"):
			time.sleep(0.2)
			line = self.serial.readline().decode('ascii')
		elapsed = float(line.strip().split(":")[1])
		line = self.serial.readline().decode('ascii')
		while (line == None or line == "\n"):
			time.sleep(0.2)
			line = self.serial.readline().decode('ascii')
		u = float(line.strip().split(":")[1])
		line = self.serial.readline().decode('ascii')
		while (line == None or line == "\n"):
			time.sleep(0.2)
			line = self.serial.readline().decode('ascii')
		x = list(map(float, line.strip().split(":")[1].split(";")))
		return (u, x, elapsed)

	def readGoldenRef(self, gold):
		with open(gold,"r") as goldenRef:
			self.initialConditions = list(map(float, goldenRef.readline().strip().split(";")))
			for tst in range(self.tests):
				data = list(map(float, goldenRef.readline().strip().split(";")))
				self.refu.append(data[0])
				self.refx.append(data[1:])

	def getMSE(self, A, B):
		res = 0
		if (not hasattr(A, "__iter__")):
			return (A - B) ** 2

		for a,b in zip(A,B):
			res += ((a - b) ** 2) / float(len(A))

		return res



	def closeSerial(self):
		self.serial.close()
		self.serial = None

	def runTest(self):
		result = 0
		self.sendVector(self.initialConditions)
		for tst in range(self.tests):
			(u, x, elapsed) = self.receiveResult()

			t_elapsed = elapsed / 325e3
			self.timeEl.append(t_elapsed)
			u_err = self.getMSE(u, self.refu[tst])
			x_err = self.getMSE(x, self.refx[tst])
			print("T: %d\t U_MSE: %.3f \tX_MSE: %.3f \tTime: %.3f ms\n" % (tst, u_err, x_err, t_elapsed))
			print("\tu: %.3f \t Expected: %.3f" % (u, self.refu[tst]))
			print("\tx: [%.3f , %.3f] \t Expected: [%.3f, %.3f]" % (x[0], x[1], self.refx[tst][0], self.refx[tst][1]))
			if (u_err > 0.1 or x_err > 0.1):
				result += 1
				print("TRIAL: %d FAILED!!" % tst)
				if result > 50:
					return result

		return result

	def toFile(self, filename):
		self.sendVector(self.initialConditions)
		fp = open(filename, 'w')
		for tst in range(3*self.tests):
			line = self.serial.readline().decode('ascii')
			while (line == None or line == '\n'):
				line = self.serial.readline().decode('ascii')
			fp.write(line.strip()+'\n')
		fp.close()


if __name__ == "__main__":
	
	zynqDev = zynqTest('COM8', 115200, 10000)
	print("Listening...")
	zynqDev.readGoldenRef("goldenReference.dat")
	res = zynqDev.runTest()
	#zynqDev.toFile('stdTest.txt')
	#res = 1
	if res:
		print(50 * "*")
		print("*" + 22 * " " + "FAIL" + 22 * " " + "*")
		print(50*"*")
	else:
		print(50 * "*")
		print("*" + 22 * " " + "PASS" + 22 * " " + "*")
		print(50*"*")
	print(res)

	zynqDev.closeSerial()
	

