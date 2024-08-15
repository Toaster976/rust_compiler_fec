fn main() {
    let mut n : bool = true;
  
    if false {
        println!("test1 {}", n);
        n = false;
    } else if (1 < 7) {
      n = true;
      println!("test2 {}", n);
    } else {
      n = false;
      println!("test3 {}", n);
    }
  }