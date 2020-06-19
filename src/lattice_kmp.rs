use crate::lattice::Lattice;

use std::collections::HashSet;
use std::collections::VecDeque;

pub struct LatticeKMP<'a> {
    pattern: Vec<&'a str>,
    cpattern: Vec<usize>,
}

impl<'a> LatticeKMP<'a> {
    /// Returns LatticeKMP with the given pattern.
    ///
    /// # Arguments
    ///
    /// * `pattern` - A word array
    ///
    /// # Example
    ///
    /// ```
    /// let pattern = vec!["幹", "細胞"];
    /// let kmp = LatticeKMP::new(pattern);
    /// ```
    pub fn new(pattern: Vec<&'a str>) -> LatticeKMP<'a> {
        let mut cpattern = vec![0];
        let mut j;
        for i in 1..pattern.len() {
            j = cpattern[i - 1];
            while j > 0 && pattern[j] != pattern[i] {
                j = cpattern[j - 1];
            }
            cpattern.push(j + if pattern[j] == pattern[i] { 1 } else { 0 });
        }
        LatticeKMP {
            pattern: pattern,
            cpattern: cpattern,
        }
    }

    /// Returns paths of found patterns.
    ///
    /// # Arguments
    ///
    /// * `lattice` - A lattice
    ///
    /// # Example
    ///
    /// ```
    /// let results = kmp.search(&lattice);
    /// ```
    pub fn search(&self, lattice: &'a Lattice) -> Vec<Vec<(&'a str, usize)>> {
        let mut added_candidates = HashSet::new();
        let mut queue = VecDeque::new();
        let mut candidates = VecDeque::new();
        let mut results = vec![];
        queue.push_back((0, 0));
        candidates.push_back(VecDeque::new());
        candidates[0].push_back(("", 0));
        while let Some(item) = queue.pop_front() {
            let candidate = candidates.pop_front().unwrap();
            if lattice.lattice[item.0].forward_main.is_none() {
                continue;
            }
            for edge in &lattice.lattice[item.0].forwards {
                let mut j = item.1;
                while j > 0 && edge.0 != self.pattern[j] {
                    j = self.cpattern[j - 1];
                }
                if edge.0 == self.pattern[j] {
                    j += 1;
                }
                let mut new_candidate = VecDeque::new();
                new_candidate.push_back(edge.clone());
                let mut k = candidate.len();
                while new_candidate.len() < j {
                    k -= 1;
                    new_candidate.push_front(candidate[k].clone());
                }
                new_candidate.push_front(("", candidate[k - 1].1));
                if j == self.pattern.len() {
                    results.push(new_candidate.clone().into_iter().collect());
                    j = self.cpattern[j - 1];
                    while new_candidate.len() > j + 1 {
                        new_candidate.pop_front();
                    }
                }
                if !added_candidates.contains(&new_candidate) {
                    added_candidates.insert(new_candidate.clone());
                    queue.push_back((edge.1, j));
                    candidates.push_back(new_candidate);
                }
            }
        }
        results
    }
}
