/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * dummyV3-profile only tests its interface rather than its functionality.
 *
 * ./dummyV3-profile ${INPUT_TO_PRINT}
 * dummyV3-profile with ${INPUT_TO_PRINT}
 */

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv)
{
  if (argc != 2)
    return EXIT_FAILURE;

  std::string input_to_print{argv[1]};

  std::cout << "dummyV3-profile with " << input_to_print << std::endl;

  return EXIT_SUCCESS;
}
