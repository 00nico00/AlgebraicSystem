/*******************************************************************
 * \file   AlgebraicSystem.hpp
 * \brief  离散数学实验
 *
 * \author 632107060321 陈龙 (1601550204@qq.com)
 * \date   October 2023
 *********************************************************************/

#pragma once

#include <cstddef>
#include <iostream>
#include <vector>
#include <format>
#include <ranges>
#include <optional>

namespace algebraic_system {

    // 封装了两个 print 和 println 分别使用 std::format 的格式输出
    template<typename... Args>
    void print(std::string_view fmt, Args &&... args) {
        std::cout << std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
    }

    template<typename... Args>
    void println(std::string_view fmt, Args &&... args) {
        std::cout << std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)) << '\n';
    }

    template<typename T>
    struct matrix {
        std::vector<std::vector<T>> _data{};
        size_t _row{};
        size_t _col{};

        matrix() = default;

        matrix(size_t row, size_t col) : _row{row}, _col{col} {
            resize(_row, _col);
        }

        std::vector<T> &operator[](size_t index) { return _data[index]; }

        const std::vector<T> &operator[](size_t index) const { return _data[index]; }

        void resize(size_t row, size_t col) {
            _data.resize(row, std::vector<T>(col));
        }

        auto begin() noexcept {
            return _data.begin();
        }

        auto end() noexcept {
            return _data.end();
        }

        auto begin() const noexcept {
            return _data.cbegin();
        }

        auto end() const noexcept {
            return _data.cend();
        }
    };

    /**
     * 定义某个数的逆元，包括左逆元和右逆元，因为逆元个数不唯一，因此使用 std::vector
     * @tparam T 类型
     */
    template<typename T>
    struct inverse_element {
        std::optional<std::vector<T>> left_inverse{std::nullopt};
        std::optional<std::vector<T>> right_inverse{std::nullopt};
    };

    /**
     * 根据集合的大小 n，处理集合内数据的输入以及运算矩阵的输入
     * @param n 集合的大小
     * @return 第一个是集合内容，第二个是运算矩阵
     */
    inline auto handle_input(size_t n) -> std::tuple<std::vector<int>, matrix<int>> {
        std::vector<int> set_s(n);
        matrix<int> calculate_matrix{n, n};

        println("请依次输入集合 S 的元素: ");
        for (size_t i = 0; i < n; i++) {
            std::cin >> set_s[i];
        }

        println("请输入一个 n x n 的矩阵元素作为运算表: ");
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                std::cin >> calculate_matrix[i][j];
            }
        }

        return std::make_tuple(set_s, calculate_matrix);
    }

    /**
     * 判断是否具有封闭性
     * @param s 集合
     * @param matrix 运算表
     * @return
     */
    inline bool is_closure(const std::vector<int> &s, const matrix<int> &matrix) {
        // 检查矩阵中某个数是否没有出现在 S 中
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = 0; j < s.size(); j++) {
                if (std::ranges::find(s, matrix[i][j]) == s.end()) {
                    // 在 S 中没有找到矩阵中某个值
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * 判断是否具有可交换性
     * @param s 集合
     * @param matrix 运算表
     * @return
     */
    inline bool is_commutable(const std::vector<int> &s, const matrix<int> &matrix) {
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = i + 1; j < s.size(); j++) {
                if (matrix[i][j] != matrix[j][i]) {
                    // 矩阵中关于主对角线的两个数不相等，因此不满足交换律
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * 判断是否具有可结合性
     * @param s 集合
     * @param matrix 运算表
     * @return
     */
    inline bool is_associative(const std::vector<int> &s, const matrix<int> &matrix) {
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = 0; j < s.size(); j++) {
                for (size_t k = 0; k < s.size(); k++) {
                    // 当 i*j*k 不等于 i*(j*k) 的时候则不满足可结合性
                    if (matrix[matrix[i][j]][k] != matrix[i][matrix[j][k]]) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    /**
     * 判断是否具有等幂性
     * @param s 集合
     * @param matrix 运算表
     * @return
     */
    inline bool is_idempotent(const std::vector<int> &s, const matrix<int> &matrix) {
        // 判断主对角线上的元素是否存在不与同样的集合列相等
        for (size_t i = 0; i < s.size(); i++) {
            if (matrix[i][i] != s[i]) {
                return false;
            }
        }

        return true;
    }

    inline void check_properties(const std::vector<int> &s, const matrix<int> &matrix) {
        println("封闭性: {}满足", is_closure(s, matrix) ? "" : "不");
        println("可交换性: {}满足", is_commutable(s, matrix) ? "" : "不");
        println("可结合性: {}满足", is_associative(s, matrix) ? "" : "不");
        println("等幂性: {}满足", is_idempotent(s, matrix) ? "" : "不");
    }

    /**
     * 判断代数系统是否存在幺元，同时获取幺元在集合的下标
     * @param s 集合
     * @param matrix 运算表
     * @return
     */
    inline auto
    try_get_identity_element_index(const std::vector<int> &s, const matrix<int> &matrix) -> std::optional<int> {
        // counts[i] 表示的是集合 S 中的第 i 个数是其他元素左幺元和右幺元的个数之和
        std::vector<int> counts(s.size());
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = 0; j < s.size(); j++) {
                // 如果 s[i] * s[j] = s[i] 那么 s[i] 的右幺元是 s[j]
                if (matrix[i][j] == s[i]) {
                    counts[j]++;
                }
                // 如果 s[i] * s[j] = s[j] 那么 s[j] 的左幺元是 s[i]
                if (matrix[i][j] == s[j]) {
                    counts[i]++;
                }
            }
        }
        // 如果一个数是其他元素的左幺元和右幺元的个数的和为 2n，那么其就是整个代数系统的幺元
        for (auto &&[index, value]: counts | std::views::enumerate) {
            if (value == s.size() * 2) {
                return static_cast<int>(index);
            }
        }

        return std::nullopt;
    }

    /**
     * 判断代数系统是否存在零元
     * @param s
     * @param matrix
     * @return
     */
    inline bool has_zero_element(const std::vector<int> &s, const matrix<int> &matrix) {
        // counts[i] 表示的是集合 S 中的第 i 个数是其他元素左零元和右零元的个数之和
        std::vector<int> counts(s.size());
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = 0; j < s.size(); j++) {
                // 如果 s[i] * s[j] = s[i] 那么 s[j] 的左零元是 s[i]
                if (matrix[i][j] == s[i]) {
                    counts[i]++;
                }
                // 如果 s[i] * s[j] = s[j] 那么 s[i] 的右幺元是 s[j]
                if (matrix[i][j] == s[j]) {
                    counts[j]++;
                }
            }
        }
        // 如果一个数是其他元素的左零元和右零元的个数的和为 2n，那么其就是整个代数系统的零元
        for (const auto &count: counts) {
            if (count == 2 * s.size()) {
                return true;
            }
        }

        return false;
    }

    /*
     * 尝试获取代数系统的左逆元和右逆元在集合 S 中的下标
     * @param s 集合
     * @param matrix 运算表
     * @return 返回一个 std::pair 分别代表左右幺元， 如果没有则返回 std::nullopt
     */
    inline auto try_get_inverse_element_index(const std::vector<int> &s,
                                              const matrix<int> &matrix)
    -> std::vector<inverse_element<int>> {
        std::vector<inverse_element<int>> inverse_element_indexes{s.size()};
        // 不存在幺元，所以每个元素都没有逆元
        auto identity_element_index = try_get_identity_element_index(s, matrix);
        if (!identity_element_index) {
            return inverse_element_indexes;
        }

        auto identity_element = s[*identity_element_index];
        for (int i = 0; i < s.size(); i++) {
            for (int j = 0; j < s.size(); j++) {
                // 如果 s[i] * s[j] == 幺元，说明 s[i] 的右逆元是 s[j]，s[j] 的左逆元是 s[i]
                if (matrix[i][j] == identity_element) {
                    // 此处保存的下标
                    if (!inverse_element_indexes[i].right_inverse.has_value()) {
                        inverse_element_indexes[i].right_inverse = std::make_optional(std::vector<int>());
                    }
                    inverse_element_indexes[i].right_inverse->push_back(j);
                    if (!inverse_element_indexes[j].left_inverse.has_value()) {
                        inverse_element_indexes[j].left_inverse = std::make_optional(std::vector<int>());
                    }
                    inverse_element_indexes[j].left_inverse->push_back(i);
                }
            }
        }

        return inverse_element_indexes;
    }

    inline void check_special_element(const std::vector<int> &s, const matrix<int> &matrix) {
        if (auto identity_element_index =try_get_identity_element_index(s, matrix); identity_element_index) {
            println("幺元存在，且为: {}", s[*identity_element_index]);
        } else {
            println("幺元不存在");
        }
        println("零元: {}存在", has_zero_element(s, matrix) ? "" : "不");

        auto inverse_element_indexes = try_get_inverse_element_index(s, matrix);
        for (auto &&[i, value]: inverse_element_indexes | std::views::enumerate) {
            if (!value.left_inverse) {
                println("{} 的左逆元不存在", s[i]);
            } else {
                print("{} 的左逆元存在，为: ", s[i]);
                for (const auto &index: *value.left_inverse) {
                    print("{} ", s[index]);
                }
                println("");
            }

            if (!value.right_inverse) {
                println("{} 的右逆元不存在", s[i]);
            } else {
                print("{} 的右逆元存在，为: ", s[i]);
                for (const auto &index: *value.right_inverse) {
                    print("{} ", s[index]);
                }
                println("");
            }
        }
    }

    /**
     * 判断代数系统是否是广群
     * @param s 集合
     * @param matrix 运算表
     * @return
     */
    inline bool is_broad_group(const std::vector<int> &s, const matrix<int> &matrix) {
        // 封闭的
        return is_closure(s, matrix);
    }

    /**
     * 判断代数系统是否为半群
     * @param s 集合
     * @param matrix 运算表
     * @return
     */
    inline bool is_semi_group(const std::vector<int> &s, const matrix<int> &matrix) {
        // 是广群且可结合
        return is_broad_group(s, matrix) && is_associative(s, matrix);
    }

    /**
     * 判断代数系统是否为独异点（含幺半群）
     * @param s
     * @param matrix
     * @return
     */
    inline bool is_monoid(const std::vector<int> &s, const matrix<int> &matrix) {
        // 是半群且存在幺元
        return is_semi_group(s, matrix) && try_get_identity_element_index(s, matrix);
    }

    /**
     * 判断代数系统是否为群
     * @param s
     * @param matrix
     * @return
     */
    inline bool is_group(const std::vector<int> &s, const matrix<int> &matrix) {
        if (!is_semi_group(s, matrix)) {
            return false;
        }

        // 检查是否每个元素都有逆元（左右逆元相等且有且只有一个）
        auto inverse_element_indexes = try_get_inverse_element_index(s, matrix);
        for (const auto &inverse_element_index: inverse_element_indexes) {
            if (!inverse_element_index.left_inverse || !inverse_element_index.right_inverse) {
                return false;
            }

            if (inverse_element_index.left_inverse->size() != 1 ||
                inverse_element_index.right_inverse->size() != 1 ||
                *inverse_element_index.left_inverse->cbegin() != *inverse_element_index.right_inverse->cbegin()) {
                return false;
            }
        }

        return true;
    }

    inline void check_group(const std::vector<int> &s, const matrix<int> &matrix) {
        if (is_broad_group(s, matrix)) {
            println("是广群，因为其满足封闭性");
        } else {
            println("不是广群，因为其不满足封闭性");
        }

        if (is_semi_group(s, matrix)) {
            println("是半群，因为其是广群且满足可结合性");
        } else {
            println("不是半群，因为其不同时是广群且满足可结合性");
        }

        if (is_monoid(s, matrix)) {
            println("是独异点（含幺半群）， 因为其是半群且存在幺元");
        } else {
            println("不是独异点（含幺半群）， 因为其不同时满足是半群且存在幺元");
        }

        if (is_group(s, matrix)) {
            println("是群，因为其是半群且每个元素都有逆元");
        } else {
            println("不是群，因为其不满足是半群且每个元素都有逆元");
        }
    }

    inline void export_main() {
        size_t n;
        println("请输入集合 S 的个数: ");
        std::cin >> n;

        auto [set_s, calculate_matrix] = handle_input(n);

        println("对于该代数系统: ");

        check_properties(set_s, calculate_matrix);

        println("---------------------------------------------------");

        check_special_element(set_s, calculate_matrix);

        println("---------------------------------------------------");

        check_group(set_s, calculate_matrix);
    }

}  // namespace AlgebraicSystem