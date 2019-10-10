**Just another TCP client/server with multithreading**

Asynchronous server receives messages from multiple clients (sent periodically, with user-defined time delay in seconds between consequent messages) and logs them. Messages are in format '[timestamp] client name'. Has no real value, just a task for an interview. This time with futures/async() instead of threads/join().