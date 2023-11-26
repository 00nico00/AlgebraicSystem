/*******************************************************************
 * \file   AlgebraicSystem.hpp
 * \brief  ��ɢ��ѧʵ��
 *
 * \author 632107060321 ���� (1601550204@qq.com)
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

    // ��װ������ print �� println �ֱ�ʹ�� std::format �ĸ�ʽ���
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
     * ����ĳ��������Ԫ����������Ԫ������Ԫ����Ϊ��Ԫ������Ψһ�����ʹ�� std::vector
     * @tparam T ����
     */
    template<typename T>
    struct inverse_element {
        std::optional<std::vector<T>> left_inverse{std::nullopt};
        std::optional<std::vector<T>> right_inverse{std::nullopt};
    };

    /**
     * ���ݼ��ϵĴ�С n�������������ݵ������Լ�������������
     * @param n ���ϵĴ�С
     * @return ��һ���Ǽ������ݣ��ڶ������������
     */
    inline auto handle_input(size_t n) -> std::tuple<std::vector<int>, matrix<int>> {
        std::vector<int> set_s(n);
        matrix<int> calculate_matrix{n, n};

        println("���������뼯�� S ��Ԫ��: ");
        for (size_t i = 0; i < n; i++) {
            std::cin >> set_s[i];
        }

        println("������һ�� n x n �ľ���Ԫ����Ϊ�����: ");
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                std::cin >> calculate_matrix[i][j];
            }
        }

        return std::make_tuple(set_s, calculate_matrix);
    }

    /**
     * �ж��Ƿ���з����
     * @param s ����
     * @param matrix �����
     * @return
     */
    inline bool is_closure(const std::vector<int> &s, const matrix<int> &matrix) {
        // ��������ĳ�����Ƿ�û�г����� S ��
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = 0; j < s.size(); j++) {
                if (std::ranges::find(s, matrix[i][j]) == s.end()) {
                    // �� S ��û���ҵ�������ĳ��ֵ
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * �ж��Ƿ���пɽ�����
     * @param s ����
     * @param matrix �����
     * @return
     */
    inline bool is_commutable(const std::vector<int> &s, const matrix<int> &matrix) {
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = i + 1; j < s.size(); j++) {
                if (matrix[i][j] != matrix[j][i]) {
                    // �����й������Խ��ߵ�����������ȣ���˲����㽻����
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * �ж��Ƿ���пɽ����
     * @param s ����
     * @param matrix �����
     * @return
     */
    inline bool is_associative(const std::vector<int> &s, const matrix<int> &matrix) {
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = 0; j < s.size(); j++) {
                for (size_t k = 0; k < s.size(); k++) {
                    // �� i*j*k ������ i*(j*k) ��ʱ��������ɽ����
                    if (matrix[matrix[i][j]][k] != matrix[i][matrix[j][k]]) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    /**
     * �ж��Ƿ���е�����
     * @param s ����
     * @param matrix �����
     * @return
     */
    inline bool is_idempotent(const std::vector<int> &s, const matrix<int> &matrix) {
        // �ж����Խ����ϵ�Ԫ���Ƿ���ڲ���ͬ���ļ��������
        for (size_t i = 0; i < s.size(); i++) {
            if (matrix[i][i] != s[i]) {
                return false;
            }
        }

        return true;
    }

    inline void check_properties(const std::vector<int> &s, const matrix<int> &matrix) {
        println("�����: {}����", is_closure(s, matrix) ? "" : "��");
        println("�ɽ�����: {}����", is_commutable(s, matrix) ? "" : "��");
        println("�ɽ����: {}����", is_associative(s, matrix) ? "" : "��");
        println("������: {}����", is_idempotent(s, matrix) ? "" : "��");
    }

    /**
     * �жϴ���ϵͳ�Ƿ������Ԫ��ͬʱ��ȡ��Ԫ�ڼ��ϵ��±�
     * @param s ����
     * @param matrix �����
     * @return
     */
    inline auto
    try_get_identity_element_index(const std::vector<int> &s, const matrix<int> &matrix) -> std::optional<int> {
        // counts[i] ��ʾ���Ǽ��� S �еĵ� i ����������Ԫ������Ԫ������Ԫ�ĸ���֮��
        std::vector<int> counts(s.size());
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = 0; j < s.size(); j++) {
                // ��� s[i] * s[j] = s[i] ��ô s[i] ������Ԫ�� s[j]
                if (matrix[i][j] == s[i]) {
                    counts[j]++;
                }
                // ��� s[i] * s[j] = s[j] ��ô s[j] ������Ԫ�� s[i]
                if (matrix[i][j] == s[j]) {
                    counts[i]++;
                }
            }
        }
        // ���һ����������Ԫ�ص�����Ԫ������Ԫ�ĸ����ĺ�Ϊ 2n����ô�������������ϵͳ����Ԫ
        for (auto &&[index, value]: counts | std::views::enumerate) {
            if (value == s.size() * 2) {
                return static_cast<int>(index);
            }
        }

        return std::nullopt;
    }

    /**
     * �жϴ���ϵͳ�Ƿ������Ԫ
     * @param s
     * @param matrix
     * @return
     */
    inline bool has_zero_element(const std::vector<int> &s, const matrix<int> &matrix) {
        // counts[i] ��ʾ���Ǽ��� S �еĵ� i ����������Ԫ������Ԫ������Ԫ�ĸ���֮��
        std::vector<int> counts(s.size());
        for (size_t i = 0; i < s.size(); i++) {
            for (size_t j = 0; j < s.size(); j++) {
                // ��� s[i] * s[j] = s[i] ��ô s[j] ������Ԫ�� s[i]
                if (matrix[i][j] == s[i]) {
                    counts[i]++;
                }
                // ��� s[i] * s[j] = s[j] ��ô s[i] ������Ԫ�� s[j]
                if (matrix[i][j] == s[j]) {
                    counts[j]++;
                }
            }
        }
        // ���һ����������Ԫ�ص�����Ԫ������Ԫ�ĸ����ĺ�Ϊ 2n����ô�������������ϵͳ����Ԫ
        for (const auto &count: counts) {
            if (count == 2 * s.size()) {
                return true;
            }
        }

        return false;
    }

    /*
     * ���Ի�ȡ����ϵͳ������Ԫ������Ԫ�ڼ��� S �е��±�
     * @param s ����
     * @param matrix �����
     * @return ����һ�� std::pair �ֱ����������Ԫ�� ���û���򷵻� std::nullopt
     */
    inline auto try_get_inverse_element_index(const std::vector<int> &s,
                                              const matrix<int> &matrix)
    -> std::vector<inverse_element<int>> {
        std::vector<inverse_element<int>> inverse_element_indexes{s.size()};
        // ��������Ԫ������ÿ��Ԫ�ض�û����Ԫ
        auto identity_element_index = try_get_identity_element_index(s, matrix);
        if (!identity_element_index) {
            return inverse_element_indexes;
        }

        auto identity_element = s[*identity_element_index];
        for (int i = 0; i < s.size(); i++) {
            for (int j = 0; j < s.size(); j++) {
                // ��� s[i] * s[j] == ��Ԫ��˵�� s[i] ������Ԫ�� s[j]��s[j] ������Ԫ�� s[i]
                if (matrix[i][j] == identity_element) {
                    // �˴�������±�
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
            println("��Ԫ���ڣ���Ϊ: {}", s[*identity_element_index]);
        } else {
            println("��Ԫ������");
        }
        println("��Ԫ: {}����", has_zero_element(s, matrix) ? "" : "��");

        auto inverse_element_indexes = try_get_inverse_element_index(s, matrix);
        for (auto &&[i, value]: inverse_element_indexes | std::views::enumerate) {
            if (!value.left_inverse) {
                println("{} ������Ԫ������", s[i]);
            } else {
                print("{} ������Ԫ���ڣ�Ϊ: ", s[i]);
                for (const auto &index: *value.left_inverse) {
                    print("{} ", s[index]);
                }
                println("");
            }

            if (!value.right_inverse) {
                println("{} ������Ԫ������", s[i]);
            } else {
                print("{} ������Ԫ���ڣ�Ϊ: ", s[i]);
                for (const auto &index: *value.right_inverse) {
                    print("{} ", s[index]);
                }
                println("");
            }
        }
    }

    /**
     * �жϴ���ϵͳ�Ƿ��ǹ�Ⱥ
     * @param s ����
     * @param matrix �����
     * @return
     */
    inline bool is_broad_group(const std::vector<int> &s, const matrix<int> &matrix) {
        // ��յ�
        return is_closure(s, matrix);
    }

    /**
     * �жϴ���ϵͳ�Ƿ�Ϊ��Ⱥ
     * @param s ����
     * @param matrix �����
     * @return
     */
    inline bool is_semi_group(const std::vector<int> &s, const matrix<int> &matrix) {
        // �ǹ�Ⱥ�ҿɽ��
        return is_broad_group(s, matrix) && is_associative(s, matrix);
    }

    /**
     * �жϴ���ϵͳ�Ƿ�Ϊ����㣨���۰�Ⱥ��
     * @param s
     * @param matrix
     * @return
     */
    inline bool is_monoid(const std::vector<int> &s, const matrix<int> &matrix) {
        // �ǰ�Ⱥ�Ҵ�����Ԫ
        return is_semi_group(s, matrix) && try_get_identity_element_index(s, matrix);
    }

    /**
     * �жϴ���ϵͳ�Ƿ�ΪȺ
     * @param s
     * @param matrix
     * @return
     */
    inline bool is_group(const std::vector<int> &s, const matrix<int> &matrix) {
        if (!is_semi_group(s, matrix)) {
            return false;
        }

        // ����Ƿ�ÿ��Ԫ�ض�����Ԫ��������Ԫ���������ֻ��һ����
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
            println("�ǹ�Ⱥ����Ϊ����������");
        } else {
            println("���ǹ�Ⱥ����Ϊ�䲻��������");
        }

        if (is_semi_group(s, matrix)) {
            println("�ǰ�Ⱥ����Ϊ���ǹ�Ⱥ������ɽ����");
        } else {
            println("���ǰ�Ⱥ����Ϊ�䲻ͬʱ�ǹ�Ⱥ������ɽ����");
        }

        if (is_monoid(s, matrix)) {
            println("�Ƕ���㣨���۰�Ⱥ���� ��Ϊ���ǰ�Ⱥ�Ҵ�����Ԫ");
        } else {
            println("���Ƕ���㣨���۰�Ⱥ���� ��Ϊ�䲻ͬʱ�����ǰ�Ⱥ�Ҵ�����Ԫ");
        }

        if (is_group(s, matrix)) {
            println("��Ⱥ����Ϊ���ǰ�Ⱥ��ÿ��Ԫ�ض�����Ԫ");
        } else {
            println("����Ⱥ����Ϊ�䲻�����ǰ�Ⱥ��ÿ��Ԫ�ض�����Ԫ");
        }
    }

    inline void export_main() {
        size_t n;
        println("�����뼯�� S �ĸ���: ");
        std::cin >> n;

        auto [set_s, calculate_matrix] = handle_input(n);

        println("���ڸô���ϵͳ: ");

        check_properties(set_s, calculate_matrix);

        println("---------------------------------------------------");

        check_special_element(set_s, calculate_matrix);

        println("---------------------------------------------------");

        check_group(set_s, calculate_matrix);
    }

}  // namespace AlgebraicSystem