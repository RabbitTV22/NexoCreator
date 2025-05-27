#include <fstream>
#include <iostream>

#include "json.hpp"
using std::cout, std::cin, std::endl, std::string;
using ordered_json = nlohmann::ordered_json;

class MainSelection {
  protected:
    string itemname = "my_item";
    string displayname = "<red>My Item";
    string material = "PAPER";
    string parentmodel = "item/handheld";
    ordered_json item;
public:
  string filename;
  string texture;
  string model;
  void setDisplayname(string display, string id) {
    itemname = id;
    displayname = display;
    item[id]["itemname"] = display;
  }
  void setMaterial(string mat) {
    material = mat;
    std::transform(mat.begin() , mat.end() , mat.begin() , ::toupper);
    item[itemname]["material"] = mat;
  }
  void setTexture(string tex) {
    texture = texture + tex;
    item[itemname]["Pack"]["texture"] = tex;
  }
  void setModel(string mod) {
    model = model + mod;
    item[itemname]["Pack"]["model"] = mod;
  }
  void setParentModel(string parent) {
    parentmodel = parent;
    item[itemname]["Pack"]["parent_model"] = parent;
  }
  // void saveMainselection() {
  //   std::ofstream file(string("output/") + filename);
  //   if (!file.is_open()) {
  //     std::cerr << "Failed to open file\n";
  //     return;
  //   }
  //   file << item.dump(2);
  // }
};


class Block: public MainSelection {
  private:
    string blockType;
public:
  void setType(int type) {
    switch (type) {
      case 1: blockType = "NOTEBLOCK"; break;
      case 2: blockType = "CHORUSBLOCK"; break;
      case 3: blockType = "STRINGBLOCK"; break;
      default: blockType = "NOTEBLOCK"; break;
    }
    item[itemname]["Mechanics"]["custom_block"]["type"] = blockType;
  }
  void setVariation(int variation) {
    item[itemname]["Mechanics"]["custom_block"]["variation"] = variation;
  }
  void setModel(int mod) {
    item[itemname]["Pack"]["model"] = mod;
  }
  void setHardness(int hardness) {
    item[itemname]["Mechanics"]["custom_block"]["hardness"] = hardness;
  }
  void setBeaconBase(bool beaconBase) {
    item[itemname]["Mechanics"]["custom_block"]["beacon_base_block"] = beaconBase;
  }
  void saveBlock() {
    std::ofstream file(string("output/") + filename);
    if (!file.is_open()) {
      std::cerr << "Failed to open file\n";
      return;
    }
    file << item.dump(2);
  }
};

int main() {
  std::ifstream settings("settings.json");
  if (!settings.is_open()) {
    std::cerr << "Failed to open settings.json" << endl;
    return 1;
  }
  nlohmann::json data;
  settings >> data;
  string texture_namespace = data["texture_namespace"];
  string model_namespace = data["model_namespace"];
  string filename = data["filename"];
  cout << endl;
  cout << "**************" << endl;
  cout << "Please choose what item you want to create:" << endl;
  cout << "1. Item" << endl;
  cout << "2. Block" << endl;
  int selection;
  cin >> selection;
  MainSelection item;
  std::filesystem::create_directories("output");
  std::ofstream file(string("output/") + filename);
  item.filename = filename;
  item.model = model_namespace;
  item.texture = texture_namespace;
  cout << "Good, Now what do you want the item id to be?" << endl;
  string input;
  string itemid;
  cin >> itemid;
  cout << "What do you want the displayname to be?" << endl;
  cin >> input;
  item.setDisplayname(input, itemid);
  cout << "What do you want the material to be?" << endl;
  cin >> input;
  item.setMaterial(input);
  cout << "What do you want the texture to be? Enter 0 to use a model instead or 1 to set a parent model before setting the texture" << endl;
  cin >> input;
  if (input == "0") {
    cout << "What do you want the model to be?" << endl;
    cin >> input;
    item.setModel(input);
  } else if (input == "1") {
    cout << "What do you want the parent model to be?" << endl;
    cin >> input;
    item.setParentModel(input);
    cout << "What do you want the texture to be?" << endl;
    cin >> input;
    item.setTexture(input);
  } else {
    item.setTexture(input);
  }
  if (selection == 1) {
   // Item item;
  } else if (selection == 2) {
    Block block;
    cout << "What type of block do you want?\n1 - NOTEBLOCK\n2 - CHORUSBLOCK\n3 - STRINGBLOCK" << endl;
    int intInput;
    cin >> intInput;
    block.setType(intInput);

    block.saveBlock();
  } else {
    cout << "Invalid selection. Please pick again." << endl;
    cin >> selection;
  }


//item.saveMainselection();

  return 0;
}
