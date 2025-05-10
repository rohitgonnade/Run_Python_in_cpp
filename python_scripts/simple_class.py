class SimpleMath:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def multiply(self):
        return self.a * self.b

    def add(self):
        return self.a + self.b

    def subtract(self):
        return self.a - self.b

    def divide(self):
        if self.b == 0:
            return "Error: Division by zero"
        return self.a / self.b


