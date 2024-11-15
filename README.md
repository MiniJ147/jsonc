# JSON-C
## Goal: Simple Json parser for C
MORE INFORMATION LATER...
WWERID BUG FOR CASES SUCH AS
```json
{
    "test":12345,,
}
```

```json
{
    "test":"12345"
}
```

it seems when they reach a certain length that is very specfic it will crash. It only happens at this length.
some small change to test something (sorry not sorry for the extra commit)
