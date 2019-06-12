import random


instructions = ["PERSON_ARRIVES", "TIME_STEP"]


def add_person():
    print(instructions[0] + " " + str(random.randint(0, 19)) + " " +
          str(random.randint(0, 19)))


def time():
    print(instructions[1])


# Generates random inputs
num_instructions = random.randint(10, 50)

add_person()
for i in range(num_instructions):
    rand_instruction = random.randint(0, 10)
    if rand_instruction < 3:
        add_person()
    else:
        time()

for i in range(int(num_instructions)):
    time
