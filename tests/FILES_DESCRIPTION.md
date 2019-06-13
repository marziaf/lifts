# Brief descriptions of the sample files provided for testing

# Manually generated

## `random`
People make 'random' movements around the floors.

This is to test that there wasn't too much bias in the implementation choices.

## `after_party`
People are going back home after a party at the 11th floor. Being a saturday night, there's still other movement around.

This case verifies the behaviour of the elevators in case of long queues.

## `check_in_day`
Some families are arriving at the hotel.

Shows the behaviour of the elevators in case of small groups coming from the same floor at different times.

## `dinner_time`
Dinner is served at floor 2.

Tests the behaviour when most of the customers are going to the same floor.

# Automatic generator

`random_generator.py` creates random configurations of inputs

Files that have been generated in this way are named `random_genX`

Example:

```
cd tests
python3 random_generator.py > random_genY
```
