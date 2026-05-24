void lab2_swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int lab2_median3_values(int x, int y, int z) {
    if ((x <= y && y <= z) || (z <= y && y <= x)) {
        return y;
    }

    if ((y <= x && x <= z) || (z <= x && x <= y)) {
        return x;
    }

    return z;
}
