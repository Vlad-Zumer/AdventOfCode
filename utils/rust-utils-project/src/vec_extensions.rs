pub trait Transposable {
    type Output;
    fn transpose(&self) -> Self::Output;
}

impl<T> Transposable for Vec<Vec<T>>
where
    T: Clone,
{
    type Output = Vec<Vec<T>>;

    fn transpose(&self) -> Self::Output {
        let mut ret_val = vec![];

        for _new_lines in 0..self[0].len()
        {
            ret_val.push(vec![]);
        }

        for y in 0..self.len()
        {
            for x in 0..self[0].len()
            {
                ret_val[x].push(self[y][x].clone());
            }
        }

        return ret_val;
    }
}
