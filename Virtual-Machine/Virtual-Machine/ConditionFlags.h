#pragma once

enum condition_flags
{
    // The condition flags represent the state of a computation's result.
    // They are stored in the last 3 bits of a 16-bit unsigned integer.
    // The flag results can be retrieved from there.

    // The POSITIVE flag indicates that the result is positive.
    FL_POSITIVE = (1 << 0),

    // The ZERO flag indicates that the result is zero.
    FL_ZERO = (1 << 1),

    // The NEGATIVE flag indicates that the result is negative.
    FL_NEGATIVE = (1 << 2)
};
