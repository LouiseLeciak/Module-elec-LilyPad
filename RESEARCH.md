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

## Things screen related

### Tearing

We will have tearing if we are not smart. It is impossible to cleanly display things if we redraw the whole screen each frame. Only time it will be acceptable to draw a full screen frame is at the setup. But then, We should only focus a precise box/area to redraw. For instance, in a clock, we a minute passes, you don't redraw the whole `12:34` but just the `4` that becomes a `5`, that way you only have to redraw a teeny tiny portion, which will be faster (say 20px x 30px = 1.2kB instead of 320px x 240px = 153.6kB)
> [!NOTE]
> The ATmega2560 has 8KB of SRAM
