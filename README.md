# Producer-Consumer

Simple program to demonstrate the producer consumer problem.  A producer thread generates an infinite amount of numbers. That number will be sorted according to whether it is prime, Fibonacci,or a multiple of 3 or 5. The producer allocates the number
to a buffer for the respective consumer and then sends a signal telling the consumer that the buffer is no longer empty. Each consumer takes the number out of the buffer and prints it to the screen in a specified format. The producer will not place an item in the buffer if an item is already present and the consumer will not try to empty an already empty buffer.
