# myOwnRedisDb
Simple redis like key-value database based on grpc protocol and bazel build system (C++17). Now db stores only string as data structure.
Some of completed features:
- [x] async - use grpc framework for this
- [x] TTL - time to live for value
- [x] snapshots - serializing all pairs of key-value pairs and possibility to deserialize it using https://uscilab.github.io/cereal/. Сreates snapshots after 60 seconds(lately, value can be confugered outside code) inactivity.
- [x] client example with few unit-tests (refactor needed)
- [x] basic operations: set, get, remove, size(return current size of db), purge(clean all values), keys(return all keys as stream)

# Run
Install bazel and then use command for server (only local now - port 50051):
```
bazel run //server:redis-server-run
```

For client-example (no CLI, just client code examples):
```
bazel run //client-example:redis-client-example-run
```
