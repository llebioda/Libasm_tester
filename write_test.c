#include "libasm_tester.h"

extern ssize_t ft_write(int fd, const void *buf, size_t count);

static t_bool write_is_valid(int fd, const void *buf, size_t count,
    const ssize_t res, const ssize_t ft_res,
    const int res_errno, const int ft_res_errno)
{
    char *std_buffer = NULL;
    char *ft_buffer = NULL;

    if (res != ft_res)
    {
        if (buf != NULL)
        {
            std_buffer = malloc(count + 1);
            if (std_buffer != NULL) strncpy(std_buffer, buf, count);
        }
        printf(RED "[WRITE] Return value mismatch on fd=%d (%s): %ld != %ld" RESET "\n", fd, std_buffer, res, ft_res);
        free(std_buffer);
        return FALSE;
    }

    if (res < 0 && res_errno != ft_res_errno)
    {
        if (buf != NULL)
        {
            std_buffer = malloc(count + 1);
            if (std_buffer != NULL) strncpy(std_buffer, buf, count);
        }
        printf(RED "[WRITE] Errno value mismatch on fd=%d (%s): %d != %d" RESET "\n", fd, std_buffer, res_errno, ft_res_errno);
        free(std_buffer);
        return FALSE;
    }

    std_buffer = malloc(count + 1);
    ft_buffer = malloc(count + 1);

    if (!std_buffer || !ft_buffer)
    {
        printf(RED "[WRITE] MALLOC ERROR on fd=%d with count=%zu" RESET "\n", fd, count);
        free(std_buffer);
        free(ft_buffer);
        return FALSE;
    }

    ssize_t std_read_res = read(fd, std_buffer, count);
    ssize_t ft_read_res = read(fd, ft_buffer, count);

    if (std_read_res != ft_read_res)
    {
        printf(RED "[WRITE] Read size mismatch on fd=%d with count=%zu : %ld != %ld" RESET "\n", fd, count, std_read_res, ft_read_res);
        free(std_buffer);
        free(ft_buffer);
        return FALSE;
    }

    std_buffer[count] = '\0';
    ft_buffer[count] = '\0';

    t_bool result = strcmp(std_buffer, ft_buffer) == 0;

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
    int fd3 = open(TMP_DIR "/test1", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd4 = open(TMP_DIR "/test2", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd5 = open(TMP_DIR "/test3", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd6 = open(TMP_DIR "/test4", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd7 = open(TMP_DIR "/test5", O_WRONLY | O_CREAT | O_TRUNC, 0644);

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
    write_test(fd4, B_1_000"AA__", 1002);
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

    int fd8 = open(TMP_DIR "/benchmark1", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd8 < 0) 
    {
        printf(RED "[WRITE] ERROR Failed to open benchmark files!" RESET "\n");
        return;
    }

    write_benchmark(fd8, "Hello World!", 12);

    close(fd8);
}