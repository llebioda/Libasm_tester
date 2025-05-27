#include "libasm_tester.h"

extern ssize_t ft_write(int fd, const void *buf, size_t count);

static char *get_writable_buf(const void *buf, size_t count)
{
    if (buf == NULL)
    {
        return strdup("null");
    }

    char *dst = malloc(count + 1);
    if (dst != NULL)
    {
        if (count <= 0)
            dst[0] = '\0';
        else
        {
            if (count >= 200)
            {
                strncpy(dst, buf, 197);
                dst[197] = '.';
                dst[198] = '.';
                dst[199] = '.';
            }
            else
                strncpy(dst, buf, count);
        }
    }

    return dst;
}

static t_bool write_is_valid(const int fd, const void *buf, const size_t count,
    const ssize_t res, const ssize_t ft_res,
    const int res_errno, const int ft_res_errno, const int errno_value)
{
    char *tmp_buf = NULL;

    if (res != ft_res)
    {
        tmp_buf = get_writable_buf(buf, count);
        printf(RED "[WRITE] Return value mismatch on fd=%d with count=%zu (%s): %ld != %ld" RESET "\n",
            fd, count, tmp_buf, res, ft_res);
        free(tmp_buf);
        return FALSE;
    }

    if (res < 0)
    {
        // Verify expected errno value
        if (errno_value != res_errno)
        {
            tmp_buf = get_writable_buf(buf, count);
            printf(RED "[WRITE] " RESET RED_ "** TEST ERROR **" RESET RED
                "\nErrno value mismatch on fd=%d with count=%zu (%s)\nexpected : %d\ngot > std: %d\n    > ft : %d" RESET "\n",
                fd, count, tmp_buf, errno_value, res_errno, ft_res_errno);
            free(tmp_buf);
            return FALSE;
        }

        if (res_errno != ft_res_errno)
        {
            tmp_buf = get_writable_buf(buf, count);
            printf(RED "[WRITE] Errno value mismatch on fd=%d with count=%zu (%s): %d != %d" RESET "\n",
                fd, count, tmp_buf, res_errno, ft_res_errno);
            free(tmp_buf);
            return FALSE;
        }

        return TRUE;
    }

    void *std_buffer = malloc(count + 1);
    void *ft_buffer = malloc(count + 1);

    if (!std_buffer || !ft_buffer)
    {
        free(std_buffer);
        free(ft_buffer);
        printf(RED "[WRITE] MALLOC ERROR on fd=%d with count=%zu" RESET "\n", fd, count);
        return FALSE;
    }

    lseek(fd, -count * 2, SEEK_CUR);
    ssize_t std_read_res = read(fd, std_buffer, count);
    ssize_t ft_read_res = read(fd, ft_buffer, count);

    if (std_read_res < 0 || ft_read_res < 0)
    {
        free(std_buffer);
        free(ft_buffer);
        tmp_buf = get_writable_buf(buf, count);
        printf(RED "[WRITE] READ ERROR on fd=%d with count=%zu (%s): std=%ld, ft=%ld, errno=%d (%s)" RESET "\n",
            fd, count, tmp_buf, std_read_res, ft_read_res, errno, strerror(errno));
        free(tmp_buf);
        return FALSE;
    }

    if (std_read_res != ft_read_res || (size_t)ft_read_res != count)
    {
        free(std_buffer);
        free(ft_buffer);
        tmp_buf = get_writable_buf(buf, count);
        printf(RED "[WRITE] Read size mismatch on fd=%d with count=%zu (%s): %ld != %ld" RESET "\n",
            fd, count, tmp_buf, std_read_res, ft_read_res);
        free(tmp_buf);
        return FALSE;
    }

    // Prevent segfault if printed
    ((char *)std_buffer)[count] = '\0';
    ((char *)ft_buffer)[count] = '\0';

    t_bool result = memcmp(std_buffer, ft_buffer, count) == 0;

    if (!result)
    {
        printf(RED "[WRITE] Buffer mismatch on fd=%d with count=%zu: (%s) != (%s)" RESET "\n",
            fd, count, get_printable_str((char *)std_buffer), get_printable_str((char *)ft_buffer));
    }

    free(std_buffer);
    free(ft_buffer);

    return result;
}

static void write_test(int fd, const void *buf, size_t count, const int errno_value)
{
    ssize_t res = write(fd, buf, count);
    int res_errno = errno;
    ssize_t ft_res = ft_write(fd, buf, count);
    int ft_res_errno = errno;

    write_is_valid(fd, buf, count, res, ft_res,
        res_errno, ft_res_errno, errno_value);
}

static void write_benchmark(int fd, const void *buf, size_t count, const int errno_value)
{
    clock_t start, end;
    double time_lib = 0.0, time_ft = 0.0;

    for (int i = 0; i < BENCHMARK_ITERATIONS; i++)
    {
        start = clock();
        ssize_t res = write(fd, buf, count);
        end = clock();
        int res_errno = errno;
        time_lib += ((double)(end - start)) / 1000;

        start = clock();
        ssize_t ft_res = ft_write(fd, buf, count);
        end = clock();
        int ft_res_errno = errno;
        time_ft += ((double)(end - start)) / 1000;

        if (!write_is_valid(fd, buf, count, res, ft_res,
                res_errno, ft_res_errno, errno_value))
        {
            printf(YELLOW "Benchmark cancelled" RESET "\n");
            return;
        }
    }

    calculate_efficiency("write", time_lib, time_ft);
}

void write_tester(void)
{
    printf("\n" PURPLE " ***** WRITE *****" RESET "\n\n");

    /* TEST */
    int fd3 = open(TMP_DIR "/test1", O_RDWR | O_CREAT | O_TRUNC, 0644);
    int fd4 = open(TMP_DIR "/test2", O_RDWR | O_CREAT | O_TRUNC, 0644);
    int fd5 = open(TMP_DIR "/test3", O_RDWR | O_CREAT | O_TRUNC, 0644);
    int fd6 = open(TMP_DIR "/test4", O_RDWR | O_CREAT | O_TRUNC, 0644);
    int fd7 = open(TMP_DIR "/test5", O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fd3 < 0 || fd4 < 0 || fd5 < 0 || fd6 < 0 || fd7 < 0) 
    {
        printf(RED "[WRITE] ERROR Failed to open test files!" RESET "\n");
        return;
    }

    write_test(fd3, "Short test", 10, 0);
    write_test(fd4, "A bit longer string for testing.", 32, 0);
    write_test(fd5, "empty string.", 0, 0);
    write_test(fd6, "Testing special characters: !@#$%^&*()_+-=", 30, 0);
    write_test(fd7, "Very large input: ......................................................................................................................................................................................................................", 200, 0);

    write_test(fd3, "First write.", 12, 0);
    write_test(fd3, "Second write!", 13, 0);
    write_test(fd3, "Final write.", 12, 0);
    write_test(fd4, B_1_000"AA_-", 1003, 0);
    write_test(fd4, A_10_000"B"A_10_000, 15000, 0);

    write_test(fd4, "Unicode test: 日本語, русский, عربى", 30, 0);

    write_test(fd5, "Line 1\nLine 2\nLine 3", 18, 0);
    write_test(fd5, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n", 14, 0);
    write_test(fd5, "\t\t\t\v\v\v\n\n\n\t\b\n\t\t\v\n\a\b\b", 19, 0);

    write_test(fd3, "This should not be written", 0, 0);
    write_test(fd6, NULL, 10, EFAULT);
    write_test(fd6, NULL, 0, 0);

    write_test(fd7, "Partial write test!", 10, 0);

    close(fd3);
    close(fd4);
    close(fd5);
    close(fd6);
    close(fd7);

    write_test(-1, "fd -1", 6, EBADF);
    write_test(-2, "fd -2", 6, EBADF);
    write_test(10, "fd 10", 6, EBADF);
    write_test(12345, "fd 12345", 9, EBADF);
    write_test(42424242, "fd 42424242", 12, EBADF);
    write_test(2147483647, "fd 2147483647", 14, EBADF);

    fd3 = open(TMP_DIR "/test1", O_RDONLY, 0644);
    write_test(fd3, "couldn't write in", 18, EBADF);
    write_test(fd3, "couldn't write in for 2 time", 29, EBADF);

    close(fd3);
    write_test(fd3, "file is closed", 15, EBADF);

    /* BENCHMARK */
    fd3 = open(TMP_DIR "/benchmark1", O_RDWR | O_CREAT | O_TRUNC, 0644);
    fd4 = open(TMP_DIR "/benchmark2", O_RDWR | O_CREAT | O_TRUNC, 0644);
    fd5 = open(TMP_DIR "/benchmark3", O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fd3 < 0 || fd4 < 0 || fd5 < 0)
    {
        printf(RED "[WRITE] ERROR Failed to open benchmark files!" RESET "\n");
        return;
    }

    write_benchmark(fd3, "", 0, 0);
    write_benchmark(fd3, "", 1, 0);
    write_benchmark(fd4, "Hello World!", 12, 0);
    write_benchmark(fd5, A_1_000".\n", 1002, 0);

    close(fd3);
    close(fd4);
    close(fd5);
}