class WordReader:
    def __init__(self, filename):
        with open(filename) as f:
            self.words = [word.strip() for line in f for word in line.split()]
            self.i = 0

    def __call__(self, type=str):
        self.i += 1
        return type(self.words[self.i - 1])


# mapper = {}
# class WordReader:
#     def __init__(self, filename, mapper):
#         with open(filename) as f:
#             self.words = [word.strip() for line in f for word in line.split()]

#             no = 0
#             for word in self.words:
#                 try:
#                     int(word)
#                 except:
#                     mapper[word] = no
#                     no += 1

#             self.i = 0

#     def __call__(self, type=str):
#         self.i += 1
#         return type(self.words[self.i - 1])

