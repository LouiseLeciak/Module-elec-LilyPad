# Research

## Things ATMega related

### ATMega328P corrupted array

Nigel and I noticed that for the following array:

```C
const uint8_t font5x7[95][5] = {0};
```

Indexes 16 to 29 were corrupted, our hypothesis was that those indexes were
somehow overlapping on a read-only memory zone.
Removing the `const` qualifier didn't solve the problem, it even led to the
whole array being corrupted without us doing any writes inside it.
We will use an ATMega2560 for our project so this issue may not hinder us for
our prototype nor our final product but still it's intriguing.

After doing the maths, 95 lines of 5 columns, composed of 8 bits makes up for
`95 * 5 * 8 = 3,800 bits`, which is 475 bytes. So we are far from hitting the
2 kilobytes limit.

Doing some research, I found out this
[electronics stackexchange](https://electronics.stackexchange.com/a/694919)
that could hint at the issue.
It could be that we may have created a stack overflow but due to the nature of
the toolchain, we can't really diagnose it at linking time.

But we tried shortening the array to bar this possibility and the problem still
occurred. So there may be something else at play here.
