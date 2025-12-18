#!/bin/bash

# Đường dẫn đến thư mục chứa test và kết quả mẫu
# Khi script đặt trong `incompleted/incompleted`, tests nằm ở `../../tests/tests`
TEST_DIR="../../tests/tests"
PARSER="./kplc"

# Kiểm tra xem file parser có tồn tại không
if [ ! -f "$PARSER" ]; then
    echo "Lỗi: Không tìm thấy file thực thi parser! Hãy chạy lệnh 'make' trước." 
    echo "(Chạy: cd \"$(dirname "$0")\" && make)"
    exit 1
fi

echo "========================================"
echo "      BẮT ĐẦU CHẠY TOÀN BỘ TEST"
echo "========================================"

PASS_COUNT=0
FAIL_COUNT=0

# Vòng lặp chạy từ 1 đến 6
for i in {1..6}
do
    INPUT_FILE="$TEST_DIR/example$i.kpl"
    EXPECTED_FILE="$TEST_DIR/result$i.txt"
    OUTPUT_TEMP="output_temp.txt"

    # Kiểm tra file input có tồn tại không
    if [ ! -f "$INPUT_FILE" ]; then
        echo "Test $i: Bỏ qua (Không tìm thấy $INPUT_FILE)"
        continue
    fi

    # Chạy parser
    "$PARSER" "$INPUT_FILE" > "$OUTPUT_TEMP"
    DIFF_RESULT=$(diff -w "$OUTPUT_TEMP" "$EXPECTED_FILE")

    if [ -z "$DIFF_RESULT" ]; then
        echo -e "Test example$i.kpl: \033[0;32m[PASSED]\033[0m"
        ((PASS_COUNT++))
    else
        echo -e "Test example$i.kpl: \033[0;31m[FAILED]\033[0m"
        echo "   -> Khác biệt tìm thấy (Output thực tế | Result mẫu):"
        diff -y --suppress-common-lines "$OUTPUT_TEMP" "$EXPECTED_FILE" | head -n 20
        ((FAIL_COUNT++))
    fi
done
rm -f "$OUTPUT_TEMP"

echo "========================================"
echo "TỔNG KẾT:"
echo -e "PASSED: \033[0;32m$PASS_COUNT\033[0m"
echo -e "FAILED: \033[0;31m$FAIL_COUNT\033[0m"
echo "========================================"