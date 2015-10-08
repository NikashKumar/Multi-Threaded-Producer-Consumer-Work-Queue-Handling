# Multi-Threaded-Producer-Consumer-Work-Queue-Handling
Implement a producer thread and a consumer thread for a work queue using pthread, mutex and conditional variable
Each work item in the queue is a string. The queue items are dynamically allocated. The size of the queue is unbounded.
The producer repeatedly: (1) reads a line from the standard input into a string, (2) enqueues the string as a single work item into the queue. The producer also wakes up the consumer if the consumer is waiting for new work items.
The consumers dequeues each string from the queue, counts the number of words in the string and then prints out both the string and its size (in words). When dequeuing , the consumer should wait if the queue is empty.
