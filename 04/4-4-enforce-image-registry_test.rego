package user.dockerfile.ID001

test_registry_denied {
  r := deny with input as {"Stages": [{
    "Name": "ubuntu:22.04",
    "Commands": [{"Cmd": "from", "Value": ["ubuntu:20.04"]}],
  }]}

  count(r) == 1
  r[_] == "This image registry is forbidden: ubuntu:20.04"
}


test_registry_allowed {
  r := deny with input as {"Stages": [{
    "Name": "ubuntu:22.04",
    "Commands": [{"Cmd": "from", "Value": ["hub.example.com/ubuntu:20.04"]}],
  }]}

  count(r) == 0
}

