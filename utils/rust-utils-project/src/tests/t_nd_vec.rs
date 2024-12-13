use nd_vec::NDVec;

#[path = "../nd_vec.rs"]
pub mod nd_vec;

#[test]
fn test_dimensions_and_size() {
    type TVec1 = NDVec<u8, 10>;
    type TVec2 = NDVec<u8, 20>;

    let zero1 = TVec1::zero();
    assert_eq!(10, TVec1::dimensions());
    assert_eq!(TVec1::dimensions(), zero1.size());
    assert_eq!(10, zero1.size());

    let zero2 = TVec2::zero();
    assert_eq!(20, TVec2::dimensions());
    assert_eq!(TVec2::dimensions(), zero2.size());
    assert_eq!(20, zero2.size());
}

#[test]
fn test_create_zero_vector() {
    type TVec1 = NDVec<u8, 10>;

    let zero = TVec1::zero();
    for i in 0..zero.size() {
        assert_eq!(
            0, zero[i],
            "Found non-zero element '{}' at position {}",
            zero[i], i
        );
    }
}

#[test]
fn test_create_vector_from_arr() {
    type TVec1 = NDVec<u8, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
    let vector = TVec1::new(arr);
    for i in 0..vector.size() {
        assert_eq!(
            arr[i], vector[i],
            "Expected '{}' but got '{}' at position {}",
            arr[i], vector[i], i
        );
    }
}

#[test]
fn test_vector_eq() {
    type TVec1 = NDVec<u8, 10>;
    let arr1 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
    let arr2 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];

    let v1 = TVec1::new(arr1);
    let _v1 = TVec1::new(arr1);
    let v2 = TVec1::new(arr2);

    assert_eq!(&v1, &v1);
    assert_eq!(&v1, &v1.clone());
    assert_eq!(&_v1, &v1);
    assert_ne!(&v1, &v2);
}

#[test]
fn test_vector_to_dimensions() {
    type TVec1 = NDVec<u8, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
    let vec10d = TVec1::new(arr);

    let vec5d = vec10d.to_dimensions::<5>();
    let expected_5d = NDVec::new([1, 2, 3, 4, 5]);
    assert_eq!(5, vec5d.size());
    assert_eq!(expected_5d, vec5d);

    let vec15d = vec10d.to_dimensions::<15>();
    let expected_15d = NDVec::new([1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0]);
    assert_eq!(15, vec15d.size());
    assert_eq!(expected_15d, vec15d);
}

#[test]
fn test_vector_as_vec() {
    type TVec1 = NDVec<u8, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
    let ndvec = TVec1::new(arr);
    let as_vec = ndvec.as_vec();
    assert_eq!(
        Vec::from(arr),
        as_vec,
        "Expected '{:?}' but got '{:?}'",
        Vec::from(arr),
        as_vec
    );
}

#[test]
fn test_vector_map() {
    type TVec1 = NDVec<u8, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
    let vector = TVec1::new(arr);
    let mapped = vector.map(|x| x * 2);
    for i in 0..arr.len() {
        assert_eq!(
            (arr[i] * 2),
            mapped[i],
            "Expected '{}' but got '{}' at position {}",
            (arr[i] * 2),
            mapped[i],
            i
        );
    }
}

#[test]
fn test_vector_fold() {
    type TVec1 = NDVec<u8, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
    let vector = TVec1::new(arr);
    let folded = vector.fold(0u64, |acc, new| acc + (*new as u64));
    // 45 = (9*10)/2
    assert_eq!(45, folded);
}

#[test]
fn test_vector_len_squared() {
    type TVec1 = NDVec<u16, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0];
    let vector = TVec1::new(arr);
    let len_squared = vector.len_squared();
    // sum(x^2, 0, N) = (n*(n+1)*(2*n+1)) / 6
    // sum(x^2, 0, 9) = (9*10*19) / 6 = 285
    assert_eq!(285, len_squared);
}

#[test]
fn test_vector_lenf32() {
    type TVec1 = NDVec<i32, 3>;

    // n^2 + (n+1)^2 + (n*(n+1))^2 = (n^2 + n + 1)^2
    // n = 6
    let arr = [6, 7, 42];
    let vector = TVec1::new(arr);
    let len_squared = vector.lenf32();
    assert_eq!(43.0, len_squared);
}

#[test]
fn test_vector_lenf64() {
    type TVec1 = NDVec<i32, 3>;

    // n^2 + (n+1)^2 + (n*(n+1))^2 = (n^2 + n + 1)^2
    // n = 2
    let arr = [6, 7, 42];
    let vector = TVec1::new(arr);
    let len_squared = vector.lenf64();
    assert_eq!(43.0, len_squared);
}

#[test]
fn test_vector_add() {
    type TVec1 = NDVec<u16, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let rev_arr = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1];
    let sum_arr = [11, 11, 11, 11, 11, 11, 11, 11, 11, 11];

    let get_vectors = || (TVec1::new(arr), TVec1::new(rev_arr));
    let expected_sum = TVec1::new(sum_arr);

    {
        let (v1, v2) = get_vectors();
        let sum = &v1 + &v2;
        assert_eq!(
            expected_sum, sum,
            "Expected '{}' but got '{}'",
            expected_sum, sum
        );
    }

    {
        let (v1, v2) = get_vectors();
        let sum = v1 + &v2;
        assert_eq!(
            expected_sum, sum,
            "Expected '{}' but got '{}'",
            expected_sum, sum
        );
    }

    {
        let (v1, v2) = get_vectors();
        let sum = &v1 + v2;
        assert_eq!(
            expected_sum, sum,
            "Expected '{}' but got '{}'",
            expected_sum, sum
        );
    }

    {
        let (v1, v2) = get_vectors();
        let sum = v1 + v2;
        assert_eq!(
            expected_sum, sum,
            "Expected '{}' but got '{}'",
            expected_sum, sum
        );
    }
}

#[test]
fn test_vector_add_assign() {
    type TVec1 = NDVec<u16, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let rev_arr = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1];
    let sum_arr = [11, 11, 11, 11, 11, 11, 11, 11, 11, 11];

    let get_vectors = || (TVec1::new(arr), TVec1::new(rev_arr));
    let expected_sum = TVec1::new(sum_arr);

    {
        let (mut actual, v2) = get_vectors();
        actual += v2;
        assert_eq!(
            expected_sum, actual,
            "Expected '{}' but got '{}'",
            expected_sum, actual
        );
    }

    {
        let (mut actual, v2) = get_vectors();
        actual += &v2;
        assert_eq!(
            expected_sum, actual,
            "Expected '{}' but got '{}'",
            expected_sum, actual
        );
    }

    {
        let (mut v1, v2) = get_vectors();
        let mut actual = &mut v1;
        actual += v2;
        assert_eq!(
            &expected_sum, actual,
            "Expected '{}' but got '{}'",
            &expected_sum, actual
        );
    }

    {
        let (mut v1, v2) = get_vectors();
        let mut actual = &mut v1;
        actual += &v2;
        assert_eq!(
            &expected_sum, actual,
            "Expected '{}' but got '{}'",
            &expected_sum, actual
        );
    }
}

#[test]
fn test_vector_sub() {
    type TVec1 = NDVec<i16, 10>;

    let lhs_arr = [11, 11, 11, 11, 11, 11, 11, 11, 11, 11];
    let rhs_arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let diff_arr = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1];

    let get_vectors = || (TVec1::new(lhs_arr), TVec1::new(rhs_arr));
    let expected_dif = TVec1::new(diff_arr);

    {
        let (v1, v2) = get_vectors();
        let dif = &v1 - &v2;
        assert_eq!(
            expected_dif, dif,
            "Expected '{}' but got '{}'",
            expected_dif, dif
        );
    }

    {
        let (v1, v2) = get_vectors();
        let dif = v1 - &v2;
        assert_eq!(
            expected_dif, dif,
            "Expected '{}' but got '{}'",
            expected_dif, dif
        );
    }

    {
        let (v1, v2) = get_vectors();
        let dif = &v1 - v2;
        assert_eq!(
            expected_dif, dif,
            "Expected '{}' but got '{}'",
            expected_dif, dif
        );
    }

    {
        let (v1, v2) = get_vectors();
        let dif = v1 - v2;
        assert_eq!(
            expected_dif, dif,
            "Expected '{}' but got '{}'",
            expected_dif, dif
        );
    }
}

#[test]
fn test_vector_sub_assign() {
    type TVec1 = NDVec<i16, 10>;

    let lhs_arr = [11, 11, 11, 11, 11, 11, 11, 11, 11, 11];
    let rhs_arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let diff_arr = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1];

    let get_vectors = || (TVec1::new(lhs_arr), TVec1::new(rhs_arr));
    let expected_dif = TVec1::new(diff_arr);
    {
        let (mut actual, v2) = get_vectors();
        actual -= v2;
        assert_eq!(
            expected_dif, actual,
            "Expected '{}' but got '{}'",
            expected_dif, actual
        );
    }

    {
        let (mut actual, v2) = get_vectors();
        actual -= &v2;
        assert_eq!(
            expected_dif, actual,
            "Expected '{}' but got '{}'",
            expected_dif, actual
        );
    }

    {
        let (mut v1, v2) = get_vectors();
        let mut actual = &mut v1;
        actual -= v2;
        assert_eq!(
            &expected_dif, actual,
            "Expected '{}' but got '{}'",
            &expected_dif, actual
        );
    }

    {
        let (mut v1, v2) = get_vectors();
        let mut actual = &mut v1;
        actual -= &v2;
        assert_eq!(
            &expected_dif, actual,
            "Expected '{}' but got '{}'",
            &expected_dif, actual
        );
    }
}

#[test]
fn test_vector_neg() {
    type TVec1 = NDVec<i16, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let neg_arr = [-1, -2, -3, -4, -5, -6, -7, -8, -9, -10];
    let v = TVec1::new(arr);
    let expected = TVec1::new(neg_arr);
    assert_eq!(
        expected,
        (-&v),
        "Expected '{}' but got '{}'",
        expected,
        (-&v)
    );
    assert_eq!(
        expected,
        (-v.clone()),
        "Expected '{}' but got '{}'",
        expected,
        (-v)
    );
}

#[test]
fn test_vector_mul() {
    type TVec1 = NDVec<i16, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let mul_arr = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20];
    let expected = TVec1::new(mul_arr);

    {
        let v = TVec1::new(arr);
        let actual = v * 2;
        assert_eq!(
            expected, actual,
            "Expected '{}' but got '{}'",
            expected, actual
        );
    }

    {
        let v = TVec1::new(arr);
        let actual = &v * 2;
        assert_eq!(
            expected, actual,
            "Expected '{}' but got '{}'",
            expected, actual
        );
    }
}

#[test]
fn test_vector_mul_assign() {
    type TVec1 = NDVec<i16, 10>;

    let arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let mul_arr = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20];
    let expected = TVec1::new(mul_arr);

    {
        let mut actual = TVec1::new(arr);
        actual *= 2;
        assert_eq!(
            expected, actual,
            "Expected '{}' but got '{}'",
            expected, actual
        );
    }

    {
        let mut v = TVec1::new(arr);
        let mut actual = &mut v;
        actual *= 2;
        assert_eq!(
            &expected, actual,
            "Expected '{}' but got '{}'",
            &expected, actual
        );
    }
}

#[test]
fn test_vector_div() {
    type TVec1 = NDVec<i16, 10>;

    let arr = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20];
    let div_arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let expected = TVec1::new(div_arr);

    {
        let v = TVec1::new(arr);
        let actual = v / 2;
        assert_eq!(
            expected, actual,
            "Expected '{}' but got '{}'",
            expected, actual
        );
    }

    {
        let v = TVec1::new(arr);
        let actual = &v / 2;
        assert_eq!(
            expected, actual,
            "Expected '{}' but got '{}'",
            expected, actual
        );
    }
}

#[test]
fn test_vector_div_assign() {
    type TVec1 = NDVec<i16, 10>;

    let arr = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20];
    let div_arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let expected = TVec1::new(div_arr);

    {
        let mut actual = TVec1::new(arr);
        actual /= 2;
        assert_eq!(
            expected, actual,
            "Expected '{}' but got '{}'",
            expected, actual
        );
    }

    {
        let mut v = TVec1::new(arr);
        let mut actual = &mut v;
        actual /= 2;
        assert_eq!(
            &expected, actual,
            "Expected '{}' but got '{}'",
            &expected, actual
        );
    }
}

#[test]
fn test_vector_abs() {
    type TVec1 = NDVec<i16, 10>;

    let arr = [-1, 2, 3, -4, 5, 6, -7, -8, -9, 10];
    let abs_arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let actual = TVec1::new(arr);
    let expected = TVec1::new(abs_arr);

    assert_eq!(
        expected,
        (&actual).abs(),
        "Expected '{}' but got '{}'",
        expected,
        actual.abs()
    );
}
