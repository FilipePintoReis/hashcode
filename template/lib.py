class WordReader:
    def __init__(self, filename):
        with open(filename) as f:
            self.words = [word.strip() for line in f for word in line.split()]
            self.i = 0
        print(self.words)

    def __call__(self, type=str):
        self.i += 1
        return type(self.words[self.i - 1])
