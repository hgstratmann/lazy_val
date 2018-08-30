# Lazy Value Initialization

A single header for lazy value initialization.

Install by copying `lazy_val.h` into your project.

## Usage

```
struct Foobar
{
    Foobar(int x, const std::string &y, bool z) : x(x), y(y), z(z) {}
    int x = 0;
    std::string y;
    bool z = false;
};

lazy_val<Foobar> foobar;
foobar.init(100, "Hello World", true); // Initialize and forward arguments to the constructor
foobar->x = 24; // Access fields with ->
foobar.ptr(); // Get raw pointer
foobar.ref(); // Get referance
```

## License

MIT Licensed.
