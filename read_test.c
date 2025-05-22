#include "libasm_tester.h"

extern ssize_t ft_read(int fd, void *buf, size_t count);

static t_bool read_is_valid(int fd, const char *expected, size_t count,
    void *std_buffer, void *ft_buffer,
    const ssize_t res, const ssize_t ft_res,
    const int res_errno, const int ft_res_errno)
{
    if (res != ft_res)
    {
        printf(RED "[READ] Return value mismatch on fd=%d with count=%zu (%s): %ld != %ld" RESET "\n",
            fd, count, expected, res, ft_res);
        return FALSE;
    }

    if (res < 0 && res_errno != ft_res_errno)
    {
        printf(RED "[READ] Errno value mismatch on fd=%d with count=%zu (%s): %d != %d" RESET "\n",
            fd, count, expected, res_errno, ft_res_errno);
        return FALSE;
    }

    // Prevent segfault if printed
    ((char *)std_buffer)[count] = '\0';
    ((char *)ft_buffer)[count] = '\0';

    // Verify expected value with result from read
    if (memcmp(expected, std_buffer, count) != 0)
    {
        printf(RED "[READ] " RESET RED_ "** TEST ERROR **" RESET RED
            "\nBuffer mismatch on fd=%d with count=%zu\nexpected : (%s)\ngot > std: (%s)\n    > ft : (%s)" RESET "\n",
            fd, count, expected, (char *)std_buffer, (char *)ft_buffer);
        return FALSE;
    }

    t_bool result = memcmp(std_buffer, ft_buffer, count) == 0 && memcmp(expected, ft_buffer, count) == 0;

    if (!result)
    {
        printf(RED "[READ] Buffer mismatch on fd=%d with count=%zu (%s): (%s) != (%s)" RESET "\n",
            fd, count, expected, (char *)std_buffer, (char *)ft_buffer);
    }

    return result;
}

static void read_test(int fd, const char *expected, size_t count)
{
    void *std_buffer = malloc(count + 1);
    void *ft_buffer = malloc(count + 1);

    if (!std_buffer || !ft_buffer)
    {
        free(std_buffer);
        free(ft_buffer);
        printf(RED "[READ] MALLOC ERROR on fd=%d with expected=[%s] and count=%zu" RESET "\n",
            fd, expected, count);
        return;
    }

    ssize_t res = read(fd, std_buffer, count);
    int res_errno = errno;
    ssize_t ft_res = ft_read(fd, ft_buffer, count);
    int ft_res_errno = errno;

    read_is_valid(fd, expected, count, std_buffer, ft_buffer,
                    res, ft_res, res_errno, ft_res_errno);
    free(std_buffer);
    free(ft_buffer);
}

static void fill_with_random_values(unsigned char *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++)
        buffer[i] = rand() % 256; // Generate a random byte (0-255)
}

static void read_benchmark(int fd, const char *expected, size_t count)
{

    void *std_buffer = malloc(count + 1);
    void *ft_buffer = malloc(count + 1);

    if (!std_buffer || !ft_buffer)
    {
        free(std_buffer);
        free(ft_buffer);
        printf(RED "[READ] MALLOC ERROR on fd=%d with expected=[%s] and count=%zu" RESET "\n",
            fd, expected, count);
        printf(YELLOW "Benchmark cancelled" RESET "\n");
        return;
    }

    clock_t start, end;
    double time_lib = 0.0, time_ft = 0.0, efficiency;

    for (int i = 0; i < BENCHMARK_ITERATIONS; i++)
    {
        start = clock();
        ssize_t res = read(fd, std_buffer, count);
        end = clock();
        int res_errno = errno;
        time_lib += ((double)(end - start)) / 1000;

        start = clock();
        ssize_t ft_res = ft_read(fd, ft_buffer, count);
        end = clock();
        int ft_res_errno = errno;
        time_ft += ((double)(end - start)) / 1000;

        if (!read_is_valid(fd, expected, count, std_buffer, ft_buffer,
                            res, ft_res, res_errno, ft_res_errno))
        {
            printf(YELLOW "Benchmark cancelled" RESET "\n");
            free(std_buffer);
            free(ft_buffer);
            return;
        }
        fill_with_random_values(std_buffer, count);
        fill_with_random_values(ft_buffer, count);
    }

    free(std_buffer);
    free(ft_buffer);

    efficiency = (time_lib / time_ft) * 100;

    const char *eff_color = (efficiency > 97.5) ? GREEN : ((efficiency > 60) ? YELLOW : ORANGE);

    printf("%s[BENCHMARK] read:%9.3f ms | ft_read:%9.3f ms | Efficiency: %.2f%%" RESET "\n", 
           eff_color, time_lib, time_ft, efficiency);
}

static t_bool write_x2(int fd, const void *buf, size_t count)
{
    if (write(fd, buf, count) < 0 || write(fd, buf, count) < 0)
    {
        printf(RED "[READ] WRITE ERROR on fd=%d with count=%zu" RESET "\n",
            fd, count);
        return FALSE;
    }
    return TRUE;
}

static t_bool write_x2_benchmark(int fd, const void *buf, size_t count)
{
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++)
    {
        if (!write_x2(fd, buf, count))
        {
            return FALSE;
        }
    }
    return TRUE;
}

void read_tester(void)
{
    printf("\n" PURPLE " ***** READ *****" RESET "\n\n");

    /* TEST */
    // Prepare data to read in test files
    int fd3 = open(TMP_DIR "/test1", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd4 = open(TMP_DIR "/test2", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd5 = open(TMP_DIR "/test3", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd6 = open(TMP_DIR "/test4", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd7 = open(TMP_DIR "/test5", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd3 < 0 || fd4 < 0 || fd5 < 0 || fd6 < 0 || fd7 < 0) 
    {
        printf(RED "[READ] ERROR Failed to open test files to write!" RESET "\n");
        return;
    }

    write_x2(fd3, "Short test", 10);
    write_x2(fd4, "A bit longer string for testing.", 32);
    write_x2(fd5, "empty string.", 0);
    write_x2(fd6, "Testing special characters: !@#$%^&*()_+-=", 30);
    write_x2(fd7, "Very large input: ......................................................................................................................................................................................................................", 200);

    write_x2(fd3, "First write.", 12);
    write_x2(fd3, "Second write!", 13);
    write_x2(fd3, "Final write.", 12);
    write_x2(fd4, B_1_000"AA_-", 1003);
    write_x2(fd4, A_10_000"B"A_10_000, 15000);

    write_x2(fd4, "Unicode test: 日本語, русский, عربى", 30);

    write_x2(fd5, "Line 1\nLine 2\nLine 3", 18);
    write_x2(fd5, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n", 14);
    write_x2(fd5, "\t\t\t\v\v\v\n\n\n\t\b\n\t\t\v\n\a\b\b", 19);

    write_x2(fd3, "This should not be written", 0);
    write_x2(fd6, "NULL1_NULL2_NULL3", 10);
    write_x2(fd6, "NULL1_NULL2_NULL3", 0);

    write_x2(fd7, "Partial write test!", 10);

    close(fd3);
    close(fd4);
    close(fd5);
    close(fd6);
    close(fd7);

    // Read test part
    fd3 = open(TMP_DIR "/test1", O_RDONLY, 0644);
    fd4 = open(TMP_DIR "/test2", O_RDONLY, 0644);
    fd5 = open(TMP_DIR "/test3", O_RDONLY, 0644);
    fd6 = open(TMP_DIR "/test4", O_RDONLY, 0644);
    fd7 = open(TMP_DIR "/test5", O_RDONLY, 0644);

    if (fd3 < 0 || fd4 < 0 || fd5 < 0 || fd6 < 0 || fd7 < 0) 
    {
        printf(RED "[READ] ERROR Failed to open test files!" RESET "\n");
        return;
    }

    read_test(fd3, "Short test", 10);
    read_test(fd4, "A bit longer string for testing.", 32);
    read_test(fd5, "empty string.", 0);
    read_test(fd6, "Testing special characters: !@#$%^&*()_+-=", 30);
    read_test(fd7, "Very large input: ......................................................................................................................................................................................................................", 200);

    read_test(fd3, "First write.", 12);
    read_test(fd3, "Second write!", 13);
    read_test(fd3, "Final write.", 12);
    read_test(fd4, B_1_000"AA_-", 1003);
    read_test(fd4, A_10_000"B"A_10_000, 15000);

    read_test(fd4, "Unicode test: 日本語, русский, عربى", 30);

    read_test(fd5, "Line 1\nLine 2\nLine 3", 18);
    read_test(fd5, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n", 14);
    read_test(fd5, "\t\t\t\v\v\v\n\n\n\t\b\n\t\t\v\n\a\b\b", 19);

    read_test(fd3, "This should not be written", 0);
    read_test(fd6, "NULL1_NULL2_NULL3", 10);
    read_test(fd6, "NULL1_NULL2_NULL3", 0);

    read_test(fd7, "Partial write test!", 10);

    close(fd3);
    close(fd4);
    close(fd5);
    close(fd6);
    close(fd7);

    /* BENCHMARK */
    // Prepare data to read in test files
    fd3 = open(TMP_DIR "/benchmark1", O_RDWR | O_CREAT | O_TRUNC, 0644);
    fd4 = open(TMP_DIR "/benchmark2", O_RDWR | O_CREAT | O_TRUNC, 0644);
    fd5 = open(TMP_DIR "/benchmark3", O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fd3 < 0 | fd4 < 0 | fd5 < 0) 
    {
        printf(RED "[READ] ERROR Failed to open benchmark files to write!" RESET "\n");
        return;
    }

    write_x2_benchmark(fd3, "", 0);
    write_x2_benchmark(fd3, "", 1);
    write_x2_benchmark(fd4, "Hello World!", 12);
    write_x2_benchmark(fd5, A_1_000".\n", 1002);

    close(fd3);
    close(fd4);
    close(fd5);

    // Read test part
    fd3 = open(TMP_DIR "/benchmark1", O_RDONLY, 0644);
    fd4 = open(TMP_DIR "/benchmark2", O_RDONLY, 0644);
    fd5 = open(TMP_DIR "/benchmark3", O_RDONLY, 0644);

    if (fd3 < 0 | fd4 < 0 | fd5 < 0) 
    {
        printf(RED "[READ] ERROR Failed to open benchmark files!" RESET "\n");
        return;
    }

    read_benchmark(fd3, "", 0);
    read_benchmark(fd3, "", 1);
    read_benchmark(fd4, "Hello World!", 12);
    read_benchmark(fd5, A_1_000".\n", 1002);
}