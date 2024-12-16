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

        for _new_lines in 0..self[0].len() {
            ret_val.push(vec![]);
        }

        for y in 0..self.len() {
            for x in 0..self[0].len() {
                ret_val[x].push(self[y][x].clone());
            }
        }

        return ret_val;
    }
}

pub trait RemoveIf<T> {
    fn remove_if<F>(&mut self, predicate: F) -> Option<T>
    where
        F: Fn(&T) -> bool;
}

impl<T> RemoveIf<T> for Vec<T> {
    fn remove_if<F>(&mut self, predicate: F) -> Option<T>
    where
        F: Fn(&T) -> bool,
    {
        self.iter()
            .position(predicate)
            .map(|index| self.remove(index))
    }
}

pub trait RemoveElem<T>
where
    T: PartialEq,
{
    fn remove_elem(&mut self, elem: &T) -> Option<T>;
}

impl<T> RemoveElem<T> for Vec<T>
where
    T: PartialEq,
{
    fn remove_elem(&mut self, elem: &T) -> Option<T> {
        self.iter()
            .position(|x| x == elem)
            .map(|index| self.remove(index))
    }
}
