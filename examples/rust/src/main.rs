use parattice::PaRattice;
use parattice::LatticeKMP;
use parattice::Lattice;

use std::fs::File;
use std::io::prelude::*;
use std::io;
use std::path::Path;

fn main() -> Result<(), io::Error> {
    // initialization
    let paradict
      = vec![vec![vec!["blood", "stem", "cell"], vec!["造血", "幹", "細胞"],
                  vec!["hematopoietic", "stem", "cell"]],
             vec![vec!["造血", "幹", "細胞", "移植"],
                  vec!["hematopoietic", "stem", "cell", "transplantation"]],
             vec![vec!["stem", "cell"], vec!["幹", "細胞"]],
             vec![vec!["幹", "細胞", "移植"], vec!["rescue", "transplant"],
                  vec!["stem", "cell", "rescue"]],
             vec![vec!["rescue"], vec!["救命"]],
             vec![vec!["blood"], vec!["血液"]]];
    let parattice = PaRattice::new(paradict);

    // lattice generation
    let words = vec!["造血", "幹", "細胞", "移植"];
    let lattice = parattice.get_lattice(&words, true, 2);

    // dump a generated lattice
    let path = Path::new("paraphrase-lattice.dot");
    let mut file = File::create(&path)?;
    file.write_all(lattice.dump_dot(true).as_bytes())?;

    // serialization & deserialization
    let bytes = lattice.to_bytes();
    let new_lattice = Lattice::new_from_bytes(&bytes);

    // search
    let kmp = LatticeKMP::new(vec!["幹", "細胞"]);
    let results = kmp.search(&new_lattice);
    for result in &results {
        for edge in result {
            print!("({}, {}) ", edge.0, edge.1);
        }
        println!();
        let s = lattice.get_trunk_span(result.clone());
        for edge in &s {
            print!("({}, {}) ", edge.0, edge.1);
        }
        println!("\n===========");
    }

    Ok(())
}
