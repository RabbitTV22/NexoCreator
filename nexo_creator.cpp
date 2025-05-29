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
    string texture;
    string model;
    ordered_json item;
public:
  string texture_namespace;
  string model_namespace;
  string filename;
  virtual void save() {
    std::ofstream file(string("output/") + filename);
    if (!file.is_open()) {
      std::cerr << "Failed to open file\n";
      return;
    }
    file << item.dump(2);
  }
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
    texture = tex;
    item[itemname]["Pack"]["texture"] = texture_namespace + tex;
  }
  void setModel(string mod) {
    model = mod;
    item[itemname]["Pack"]["model"] = model_namespace + mod;
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
  void setBlockModel(int mod) {
    item[itemname]["Mechanics"]["custom_block"]["model"] = mod;
  }
  void setHardness(int hardness) {
    item[itemname]["Mechanics"]["custom_block"]["hardness"] = hardness;
  }
  void setBeaconBase(bool beaconBase) {
    item[itemname]["Mechanics"]["custom_block"]["beacon_base_block"] = beaconBase;
  }
};

class Item: public MainSelection {
private:
  string durability;
public:
  void setDurability(int dur) {
    durability = dur;
    item[itemname]["Components"]["durability"] = dur;
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
  string filename = data["filename"];
  cout << endl;
  cout << "**************" << endl;
  cout << "Please choose what item you want to create:" << endl;
  cout << "1. Item" << endl;
  cout << "2. Block" << endl;
  int selection;
  cin >> selection;
  MainSelection* item;
  if (selection == 1) {
    item = new Item();
  } else if (selection == 2) {
    item = new Block();
  } else {
    cout << "Invalid selection. Please pick again." << endl;
  }
  item->texture_namespace = data["texture_namespace"];
  item->model_namespace = data["model_namespace"];
  std::filesystem::create_directories("output");
  std::ofstream file(string("output/") + filename);
  item->filename = filename;
  cout << "Good, Now what do you want the item id to be?" << endl;
  string input;
  string itemid;
  cin >> itemid;
  cout << "What do you want the displayname to be?" << endl;
  cin.ignore();
  getline(cin, input);
  //cin >> input;
  item->setDisplayname(input, itemid);
  cout << "What do you want the material to be?" << endl;
  cin >> input;
  item->setMaterial(input);
  cout << "What do you want the texture to be? Enter 0 to use a model instead or 1 to set a parent model before setting the texture" << endl;
  cin >> input;
  item->save();
  if (input == "0") {
    cout << "What do you want the model to be?" << endl;
    cin >> input;
    item->setModel(input);
  } else if (input == "1") {
    cout << "What do you want the parent model to be?" << endl;
    cin >> input;
    item->setParentModel(input);
    cout << "What do you want the texture to be?" << endl;
    cin >> input;
    item->setTexture(input);
  } else {
    item->setTexture(input);
  }
  if (Item* I = dynamic_cast<Item*>(item)) {
    cout << "What do you want the durability to be?" << endl;
    int dur;
    cin >> dur;
    I->setDurability(dur);
    I->save();
  } else if (Block* B = dynamic_cast<Block*>(item)) {
    cout << "What type of block do you want?\n1 - NOTEBLOCK\n2 - CHORUSBLOCK\n3 - STRINGBLOCK" << endl;
    int intInput;
    cin >> intInput;
    B->setType(intInput);

    B->save();
  }
  delete item;

  return 0;
}
