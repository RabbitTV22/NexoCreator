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
    if (hardness!=0) {
      item[itemname]["Mechanics"]["custom_block"]["hardness"] = hardness;
    }
  }
  void setBeaconBase(bool beaconBase) {
    if (beaconBase!=0) {
      item[itemname]["Mechanics"]["custom_block"]["beacon_base_block"] = beaconBase;
    }
  }
};

class Item: public MainSelection {
private:
  string hideTT = "false";
  int durability = 400;
  string EnchGlintOvvride = "false";
  int maxStack = 64;
public:
  void setHideTooltip(string hideTooltip) {
    hideTT = hideTooltip;
    item[itemname]["Components"]["hide_tooltip"] = hideTooltip;
  }
  void setEnchantmentGlintOverride(string enchantment_glint_override) {
    EnchGlintOvvride = enchantment_glint_override;
    if (enchantment_glint_override!="0") {
      item[itemname]["Components"]["enchantment_glint_override"] = enchantment_glint_override;
    }
  }
  void setDurability(int dur) {
    durability = dur;
    if (dur !=0) {
      item[itemname]["Components"]["durability"] = dur;
    }
  }
  void setMaxStackSize(int maxStackSize) {
    maxStack = maxStackSize;
    if (maxStack!=0) {
      item[itemname]["Components"]["max_stack_size"] = maxStack;
    }
  }


    void setTool(double damage_per_block, double default_mining_speed, double speed, string correct_for_drops, const std::vector<string>& materials, std::vector<string>& tags) {
      auto& tool = item[itemname]["Components"]["tool"];
      tool["rules"] = nlohmann::json::array();
      ordered_json rule = nlohmann::json::object();
    item[itemname]["Components"]["tool"]["rules"] = nlohmann::json::array();
    if (damage_per_block != 0) {
        item[itemname]["Components"]["tool"]["damage_per_block"] = damage_per_block;
      }
      if (default_mining_speed != 0) {
        item[itemname]["Components"]["tool"]["default_mining_speed"] = default_mining_speed;
      }
      if (speed != 0) {
        rule["speed"] = speed;
      }
    if (materials.size() == 1) {
        rule["material"] = materials;
    } else if (materials.size() != 1) {
      rule["materials"] = materials;
    }
    if (tags.size() == 1) {
      rule["tag"]= tags;
    } else if (tags.size() != 1){
      rule["tags"] = tags;
    }
      rule["correct_for_drops"] = correct_for_drops;
      tool["rules"].emplace_back(rule);

    }


    void setFood(double nutrition, double saturation, string can_aways_eat) {
      item[itemname]["Components"]["food"]["nutrition"] = nutrition;
      item[itemname]["Components"]["food"]["saturation"] = saturation;
      item[itemname]["Components"]["food"]["can_always_eat"] = can_aways_eat;
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
    cout << "Invalid selection." << endl;
    return 1;
  }
  item->texture_namespace = data["texture_namespace"];
  item->model_namespace = data["model_namespace"];
  std::filesystem::create_directories("output");
  std::ofstream file(string("output/") + filename);
  item->filename = filename;
  cout << "*Good, Now what do you want the item id to be?" << endl;
  string input;
  string itemid;
  cin >> itemid;
  cout << "*What do you want the displayname to be?" << endl;
  cin.ignore();
  getline(cin, input);
  //cin >> input;
  item->setDisplayname(input, itemid);
  cout << "*What do you want the material to be?" << endl;
  cin >> input;
  item->setMaterial(input);
  cout << "*What do you want the texture to be? Enter 1 to use a model instead or 2 to set a parent model before setting the texture" << endl;
  cin >> input;
  if (input == "1") {
    cout << "*What do you want the model to be?" << endl;
    cin >> input;
    item->setModel(input);
  } else if (input == "2") {
    cout << "*What do you want the parent model to be?" << endl;
    cin >> input;
    item->setParentModel(input);
    cout << "*What do you want the texture to be?" << endl;
    cin >> input;
    item->setTexture(input);
  } else {
    item->setTexture(input);
  }
  item->save();
  if (Item* I = dynamic_cast<Item*>(item)) {
    cout << "Choose what to add next\n" << "1 - Components\n2 - Lore\n3 - ItemFlags\n4 - Potion Effects\n5 - View other settings\n";
    cout << "Note that at any time you can enter 0 to not set a certain setting\n";
    cout << "Questions starting with * need to be answered.\n";
    cin >> selection;
    if (selection == 1) {
      cout << "What component(s) do you want to use? Enter 1 to set each one." << endl;
      cout << "1 - hide_tooltip\n2 - enchantment_glint_override\n3 - durability\n4 - max_stack_size\n5 - tool\n6 - food\n7 - consumable\n8 - damage_resistant\n";
      cout << "9 - enchantable\n10 - glider\n11 - item_model\n12 - tooltip_style\n13 - use_cooldown\n14 - reparable\n15 - equippable\n";
      cin >> selection;
      int intInput;
      switch (selection) {
        case 1: {
          cout << "Hide tooltip? (true/false)" << endl;
          string hideTooltip;
          cin >> hideTooltip;
          I->setHideTooltip(hideTooltip);
          break;
        }
        case 2: {
          cout << "Do you want enchantement glint? (true/false)" << endl;
          string enchantglint;
          cin >> enchantglint;
          I->setEnchantmentGlintOverride(enchantglint);
          break;
        }
        case 3: {
          cout << "What do you want the durability to be? (integer)" << endl;
          cin >> intInput;
          I->setDurability(intInput);
          break;
        }
        case 4: {
          cout << "What do you want the Maximum stack size to be? (integer)" << endl;
          cin >> intInput;
          I->setMaxStackSize(intInput);
          break;
        }
        case 5: {
          double damage, dSpeed, speed;
          std::vector<std::string> materials, tags;
          string dropsCorrect;
          string material, tag;
          cout << "What do you want the damage per block to be? (double)" << endl;
          cin >> damage;
          cout << "What do you want to be the default minging speed? (double)" << endl;
          cin >> dSpeed;
          cout << "What do you want to be the speed of this tool? (double)" << endl;
          cin >> speed;
          cout << "Do you want the tool to be correct for drops? (true/false)" << endl;
          cin >> dropsCorrect;
            cout << "What materials do you want to be allowed to mine? Enter done to finish the list." << endl;
            while (true) {
              cin >> material;
              if (material == "done") break;
              materials.push_back(material);
            }
            cout << "What do you want the block tags to be? Enter done to finish the list." << endl;
            while (true) {
              cin >> tag;
              if (tag == "done") break;
              tags.push_back(tag);
            }
          I->setTool(damage, dSpeed, speed, dropsCorrect, materials, tags);
          break;
        }
        case 6: {
          cout << "How much nutrition do you want this item to give? " << endl;
          double nutrition;
          cin >> nutrition;
          cout << "How much saturation do you want this item to give?" << endl;
          double saturation;
          cin >> saturation;
          cout << "Do you want this item to always be edible?" << endl;
          string edible;
          cin >> edible;
          I->setFood(nutrition, saturation, edible);
          break;
        } //next cases TODO
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:

        default:;
      }
    }
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
