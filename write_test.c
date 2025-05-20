#include "libasm_tester.h"

extern ssize_t ft_write(int fd, const void *buf, size_t count);

static char *get_writable_buf(const void *buf, size_t count)
{
    char *dst = NULL;

    if (buf != NULL)
    {
        dst = malloc(count + 1);
        if (count <= 0)
            dst[0] = '\0';
        else if (dst != NULL)
            strncpy(dst, buf, count);
    }

    return dst == NULL ? strdup("null") : dst;
}

static t_bool write_is_valid(int fd, const void *buf, size_t count,
    const ssize_t res, const ssize_t ft_res,
    const int res_errno, const int ft_res_errno)
{
    char *std_buffer = NULL;
    char *ft_buffer = NULL;

    if (res != ft_res)
    {
        std_buffer = get_writable_buf(buf, count);
        printf(RED "[WRITE] Return value mismatch on fd=%d with count=%zu (%s): %ld != %ld" RESET "\n", fd, count, std_buffer, res, ft_res);
        free(std_buffer);
        return FALSE;
    }

    if (res < 0 && res_errno != ft_res_errno)
    {
        std_buffer = get_writable_buf(buf, count);
        printf(RED "[WRITE] Errno value mismatch on fd=%d with count=%zu (%s): %d != %d" RESET "\n", fd, count, std_buffer, res_errno, ft_res_errno);
        free(std_buffer);
        return FALSE;
    }

    std_buffer = malloc(count + 1);
    ft_buffer = malloc(count + 1);

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
        std_buffer = get_writable_buf(buf, count);
        printf(RED "[WRITE] READ ERROR on fd=%d with count=%zu (%s): std=%ld, ft=%ld, errno=%d (%s)" RESET "\n", fd, count, std_buffer, std_read_res, ft_read_res, errno, strerror(errno));
        free(std_buffer);
        return FALSE;
    }

    if (std_read_res != ft_read_res)
    {
        free(std_buffer);
        free(ft_buffer);
        std_buffer = get_writable_buf(buf, count);
        printf(RED "[WRITE] Read size mismatch on fd=%d with count=%zu (%s): %ld != %ld" RESET "\n", fd, count, std_buffer, std_read_res, ft_read_res);
        free(std_buffer);
        return FALSE;
    }

    std_buffer[std_read_res] = '\0';
    ft_buffer[ft_read_res] = '\0';

    t_bool result = strcmp(std_buffer, ft_buffer) == 0;

    if (!result)
    {
        printf(RED "[WRITE] Buffer mismatch on fd=%d with count=%zu: (%s) != (%s)" RESET "\n", fd, count, std_buffer, ft_buffer);
    }

    free(std_buffer);
    free(ft_buffer);

    return result;
}

static void write_test(int fd, const void *buf, size_t count)
{
    ssize_t res = write(fd, buf, count);
    int res_errno = errno;
    ssize_t ft_res = ft_write(fd, buf, count);
    int ft_res_errno = errno;

    write_is_valid(fd, buf, count, res, ft_res, res_errno, ft_res_errno);
}

static void write_benchmark(int fd, const void *buf, size_t count)
{
    clock_t start, end;
    double time_lib = 0.0, time_ft = 0.0, efficiency;

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

        if (!write_is_valid(fd, buf, count, res, ft_res, res_errno, ft_res_errno))
        {
            printf(RED "Benchmark cancelled" RESET "\n");
            return;
        }
    }

    efficiency = (time_lib / time_ft) * 100;

    const char *eff_color = (efficiency > 97.5) ? GREEN : ((efficiency > 60) ? YELLOW : ORANGE);

    printf("%s[BENCHMARK] write:%9.3f ms | ft_write:%9.3f ms | Efficiency: %.2f%%" RESET "\n", 
           eff_color, time_lib, time_ft, efficiency);
}

void write_tester(void)
{
    printf("\n" PURPLE " ***** WRITE *****" RESET "\n\n");

    // Creating test files
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

    write_test(fd3, "Short test", 10);
    write_test(fd4, "A bit longer string for testing.", 32);
    write_test(fd5, "empty string.", 0);
    write_test(fd6, "Testing special characters: !@#$%^&*()_+-=", 30);
    write_test(fd7, "Very large input: ......................................................................................................................................................................................................................", 200);

    write_test(fd3, "First write.", 12);
    write_test(fd3, "Second write!", 13);
    write_test(fd3, "Final write.", 12);
    write_test(fd4, B_1_000"AA_-", 1003);
    write_test(fd4, A_10_000"B"A_10_000, 15000);

    write_test(fd4, "Unicode test: 日本語, русский, عربى", 30);

    write_test(fd5, "Line 1\nLine 2\nLine 3", 18);
    write_test(fd5, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n", 14);
    write_test(fd5, "\t\t\t\v\v\v\n\n\n\t\b\n\t\t\v\n\a\b\b", 19);

    write_test(fd3, "This should not be written", 0);
    write_test(fd6, NULL, 10);
    write_test(fd6, NULL, 0);

    write_test(fd7, "Partial write test!", 10);

    close(fd3);
    close(fd4);
    close(fd5);
    close(fd6);
    close(fd7);

    fd3 = open(TMP_DIR "/benchmark1", O_RDWR | O_CREAT | O_TRUNC, 0644);
    fd4 = open(TMP_DIR "/benchmark2", O_RDWR | O_CREAT | O_TRUNC, 0644);
    fd5 = open(TMP_DIR "/benchmark3", O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fd3 < 0 | fd4 < 0 | fd5 < 0) 
    {
        printf(RED "[WRITE] ERROR Failed to open benchmark files!" RESET "\n");
        return;
    }

    write_benchmark(fd3, "", 0);
    write_benchmark(fd3, "", 1);
    write_benchmark(fd4, "Hello World!", 12);
    write_benchmark(fd5, A_1_000".\n", 1002);

    close(fd3);
}